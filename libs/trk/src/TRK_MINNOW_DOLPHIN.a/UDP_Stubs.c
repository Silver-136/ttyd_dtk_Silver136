__declspec(weak) int udp_cc_initialize(void* flagOut, void* func) { return -1; }

__declspec(weak) int udp_cc_shutdown(void) { return -1; }

__declspec(weak) int udp_cc_open(void) { return -1; }

__declspec(weak) int udp_cc_close(void) { return -1; }

__declspec(weak) int udp_cc_read(unsigned char* buf, int len) { return 0; }

__declspec(weak) int udp_cc_write(const unsigned char* buf, int len) { return 0; }

__declspec(weak) int udp_cc_peek(void) { return 0; }

__declspec(weak) int udp_cc_pre_continue(void) { return -1; }

__declspec(weak) int udp_cc_post_stop(void) { return -1; }
