set DST_DIR=.\
set SRC_DIR=.\proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/types.proto
protoc -I=%SRC_DIR% --cpp_out=%DST_DIR% %SRC_DIR%/test.proto