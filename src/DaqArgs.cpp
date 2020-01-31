#include "DaqArgs.h"
#include <getopt.h>
#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <stdexcept>

// looks like CS111 finally paid off
struct option long_options[] = {
    {"config", required_argument, 0, 'c'},
    {"autoRecord", no_argument, 0, 'a'},
    {"realtime", no_argument, 0, 'r'},
    {0, 0, 0, 0}
};

DaqArgs::DaqArgs(int argc, char** argv) {
    char c;
    // pass 1, only syntax check
    while ((c = getopt_long(argc, argv, "c:ar", long_options, NULL)) != -1) {
        switch (c) {
            case 'c':
                configPath = std::string(optarg);
                break;
            case 'a':
                autoRecord = true;
                break;
            case 'r':
                realtime = true;
                break;
            default:
                throw std::runtime_error("Invalid argument detected");
        }
    }
    if (optind != argc) {
        throw std::runtime_error("Extraneous argument detected");
    }

    struct passwd* user = getpwuid(getuid());
    if (user == nullptr) {
        return;
    }
    username = user->pw_name;
    if (configPath[0] == '~') {
        configPath = user->pw_dir + configPath.substr(1);
    }
}