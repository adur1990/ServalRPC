#include "rhizome.h"
#include "serval.h"
#include "server.h"
#include "conf.h"
#include "commandline.h"
#include "mdp_client.h"
#include "msp_client.h"
#include "dataformats.h"
#include "cJSON.h"
#include <curl/curl.h>

#define RPC_PKT_DISCOVER 0
#define RPC_PKT_DISCOVER_ACK 1
#define RPC_PKT_CALL 2
#define RPC_PKT_CALL_ACK 3
#define RPC_PKT_CALL_RESPONSE 4

struct RPCProcedure {
    char *return_type;
    char *name;
    uint16_t paramc;
    char **params;
    sid_t caller_sid;
};

// General.
uint8_t *rpc_result[126];

// Server part.
int rpc_listen ();

// Client part.
int rpc_call (const sid_t server_sid, const char *rpc_name, const int paramc, const char **params);

// cURL helpers.
struct CurlResultMemory {
  char *memory;
  size_t size;
};

size_t _curl_write_response (void *contents, size_t size, size_t nmemb, void *userp);

void _curl_init_memory (struct CurlResultMemory *curl_result_memory);
void _curl_reinit_memory (struct CurlResultMemory *curl_result_memory);
void _curl_free_memory (struct CurlResultMemory *curl_result_memory);

void _curl_set_basic_opt (char* url, CURL *curl_handler, struct curl_slist *header);

// General helpers.
char* _rpc_flatten_params (const int paramc, const char **params);
uint8_t *_rpc_prepare_call_payload (uint8_t *payload, const int paramc, const char *rpc_name, const char *flat_params);
size_t _rpc_write_tmp_file (char *file_name, void *content, size_t len);
