#pragma once

#define SELECT_TIMEOUT_SEC   30
#define PACKET_SIZE          1460
#define PACKET_REMAIN_SIZE   2920

#define INVALID_SERIAL_ID    -1
#define MAX_IP_LEN           32
#define PORT_NUM             5000



typedef enum E_SERVER_TYPE 
{
	CONTENTS_SERVER_TYPE = 1,
	CHATING_SERVER_TYPE
} ServerType;
