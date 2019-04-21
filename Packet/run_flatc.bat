set DST_DIR=.\
set SRC_DIR=.\flat

flatc --cpp -o %DST_DIR% %SRC_DIR%/LoginReq.fbs
flatc --cpp -o %DST_DIR% %SRC_DIR%/LoginAck.fbs
flatc --cpp -o %DST_DIR% %SRC_DIR%/MessageNfy.fbs
flatc --cpp -o %DST_DIR% %SRC_DIR%/MessageReq.fbs