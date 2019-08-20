### Reactor网络框架
------
#### 线程模型
  * 单个Acceptor Loop线程
    * Acceptor Loop线程负责不断在listen fd上accept新到来的连接，并回调TcpServer实例的new_connection函数，创建新的TcpConn对象并为其分配一个IO Event Loop实例
    * 每个连接断开的时候，要在TcpServer中解注册TcpConn，为了避免加锁，各个IO线程可以把解注册TcpConn的操作封装成function<>并压入Acceptor Event Loop的pending functor队列，由Acceptor Loop执行解注册动作，用并发转串行避免锁
  * N个I/O Loop线程
    * 每个IO Loop处理多个fd上的读、写、关闭动作，每次epoll循环后执行各自pending functor队列中的functions（由worker线程压入）
  * M个worker线程
    * 每个TcpConn上读到一次完整的请求数据包后（缓存在输入缓冲中），将具体的请求处理过程（可能较耗时，不能在IO线程中做，不然影响其他fd及时读写数据）传递给worker线程池中的一个worker线程去处理。处理完后，woker线程不能直接将响应数据写到TcpConn的输出缓冲（IO线程在epoll_wait后也可能操作输出缓冲，会有竞态），而是要把send操作封装成function压入TcpConn所属的IO Event Loop的pending functor队列，即每个TcpConn的输入缓冲、输出缓冲的操作只能是在IO线程完成。
#### 对象生命周期管理
  * 使用RAII方式将fd封装在Socket类中，只有Socket实例析构的时候才执行close(fd)，Socket实例由TcpConn用unique_ptr直接管理，即每个fd只有在每个tcp连接上完成了全部的读、写、对端关闭了连接、TcpConn析构的时候，才会关闭。这样可避免串话问题。
  * TcpServer实例、Acceptor Event Loop、IO Event Loop、Worker线程池的生命期等同于网络服务进程，可以作为main函数的栈上对象存在。
  * IO事件分发器Channel对象的生命期由TcpConn用unique_ptr控制。
  * TcpConn的生命期是模糊的，使用shared_ptr控制，用到TcpConn的地方持有其shared_ptr的一份拷贝，只有引用计数为0的时候才析构它。（例如，一个tcp长连接上，对端发送若干请求报文后，关闭连接，IO Event Loop在该fd上读到0之后触发连接关闭逻辑，TcpConn的close_callback对应的是TcpServer实例中的remove_connection函数。由于该连接上还有请求未处理完，TcpConn解注册后不能马上析构，否则worker线程处理完请求后再操作TcpConn的输出缓冲时会coredump。用shared_ptr管理不会有这样的问题）
#### 主要数据流程
  * 从fd上读数据：IO Event Loop将fd内核inode输入缓存中的数据读到TcpConn的输入缓存，读的时候尽可能多读，但又不能为每个TcpConn缓存开太大的堆内存，可以用readv结合栈上内存来解决。每次读取后都要调用用户设置的回调，判断TcpConn输入缓存中的数据是否是完整的请求报文，是的话将一个完整的请求报文截取出来，和请求处理function一起传递给worker线程去处理
  * 将数据写入fd：worker线程完成请求处理后，将响应数据的发送过程封装成function，压入fd所属的IO Event Loop的pending functor队列。IO线程处理发送过程时，如果此时TcpConn的输出缓存为空且并未在epoll上关注fd的写事件，则可以直接尝试write响应数据到fd的内核inode输出缓存。如果没有一次write完，或者写之前TcpConn的输出缓存不为空，则只能将数据追加到TcpConn的输出缓存，避免乱序。如果已经将TcpConn的输出缓存的数据全部写完，则可以暂时先不关注fd在epoll上的写事件，避免busy-loop（等什么时候TcpConn输出缓存有数据了再去关注写事件）
    * 写数据过程中，如果对端发送数据较快但接收数据较慢，本地fd对应的TCP滑动窗口运动较慢，内核inode输出缓存可能一直积压数据，进而TcpConn的输出缓存不断增长，会有风险，最好设置个回调，在TcpConn输出缓存增长到警戒水位的时候触发报警
  * 关闭连接：服务器端可以主动关闭连接，只用shutdown关闭写端（必须等TcpConn输出缓存的数据全部写入fd才调用shutdown），对端read到0之后一般会调用close，服务端从fd上read到0，触发关闭连接的逻辑（在TcpServer中解注册TcpConn、在Poller中解注册Channel、等到引用计数为0的时候析构TcpConn、关闭fd）
