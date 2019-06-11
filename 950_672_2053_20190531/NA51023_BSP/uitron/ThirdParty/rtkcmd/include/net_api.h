#ifndef NET_API_H
#define NET_API_H

#define NULL_FILE 0
#define NULL_STR ""

int is_interface_up(const char *ifname);
int interface_up(const char *intf);
int interface_down(const char *intf);
int interface_config(const char *intf, char *addr, char *netmask); //SIOCSIFADDR
int interface_config2(const char *intf, char *addr, char *netmask);//SIOCAIFADDR
int shutdown_all_interfaces(void);
//void show_network(void);
int set_mac_address( const char *interface, char *mac_address );
int get_mac_address( const char *interface, char *mac_address );
void install_exception_handler(void);
int RunSystemCmd(char *filepath, ...);

#endif /* NET_API_H */
