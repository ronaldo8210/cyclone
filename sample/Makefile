CUR_DIR=/work_space/cyclone

CORE_DIR=${CUR_DIR}/core
NETWORK_DIR=${CUR_DIR}/network
SAMPLE_DIR=${CUR_DIR}/sample
COMMON_DIR=${CUR_DIR}/common
POOL_DIR=${CUR_DIR}/pool

BOOST_INC_DIR=/usr/include/boost
BOOST_LIB_DIR=/usr/lib

INC_DIR= -I${CUR_DIR} \
		 -I${CORE_DIR} \
		 -I${NETWORK_DIR} \
		 -I${COMMON_DIR} \
		 -I${POOL_DIR} \
		 -I${BOOST_INC_DIR}

SRC = ${wildcard ${CORE_DIR}/*.cpp} \
	  ${wildcard ${NETWORK_DIR}/*.cpp} \
	  ${wildcard ${COMMON_DIR}/*.cpp} \
      ${wildcard ${POOL_DIR}/*.cpp} \
	  ${wildcard ${SAMPLE_DIR}/main.cpp}
OBJ = ${patsubst %.cpp, %.o, ${SRC}}

TARGET=dsp_engine
CC=g++
CCFLAGS=-g -Wall -std=c++11 -L${BOOST_LIB_DIR} -lpthread ${INC_DIR}

${TARGET}: ${OBJ}
	${CC} ${OBJ} -std=c++11 -pthread -Wl,--no-as-needed -o $@
	@echo "compile success!!"

${OBJ}:%.o:%.cpp
	@echo "compiling $< ==> $@"
	${CC} ${CCFLAGS} -c $< -o $@

clean:
	@rm -f ${OBJ}
	@echo "clean object files done."

	@rm -f *~
	@echo "clean tempreator files done."

	@rm -f ${TARGET}
	@echo "clean target files done."

	@echo "clean complete."
