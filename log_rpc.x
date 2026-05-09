struct log_request {
    string user<255>;
    string operation<32>;
    string filename<255>;
};

program LOG_RPC_PROG {
    version LOG_RPC_VERS {
        int LOG_OPERATION(log_request) = 1;
    } = 1;
} = 0x20000099;
