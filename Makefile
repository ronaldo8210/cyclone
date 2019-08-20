CUR_DIR=/work_space/cyclone

CORE_DIR=${CUR_DIR}/core
NETWORK_DIR=${CUR_DIR}/network
COMMON_DIR=${CUR_DIR}/common
POOL_DIR=${CUR_DIR}/pool
POLLER_DIR=${CUR_DIR}/poller
LIB_DIR=${CUR_DIR}/lib

INC_DIR= -I${CUR_DIR} \
		 -I${CORE_DIR} \
		 -I${NETWORK_DIR} \
		 -I${COMMON_DIR} \
		 -I${POOL_DIR} \
		 -I${POLLER_DIR}

SRC = ${wildcard ${CORE_DIR}/*.cpp} \
	  ${wildcard ${NETWORK_DIR}/*.cpp} \
	  ${wildcard ${COMMON_DIR}/*.cpp} \
      ${wildcard ${POOL_DIR}/*.cpp} \
      ${wildcard ${POLLER_DIR}/*.cpp} 
OBJ = ${patsubst %.cpp, %.o, ${SRC}}

TARGET=libcyclone.a
CC=g++
CCFLAGS=-g -Wall -std=c++11 -lpthread ${INC_DIR}

all:${OBJ}
	ar rcs $(TARGET) $^
	mv ${TARGET} ${LIB_DIR}
	@echo "compile success!!"

${OBJ}:%.o:%.cpp
	@echo "compiling $< ==> $@"
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	@rm -f ${OBJ}
	@echo "clean object files done."

	@rm -f *~
	@echo "clean tempreator files done."

	@rm -f ${LIB_DIR}/${TARGET}
	@echo "clean .lib file done."

	@echo "clean complete."
