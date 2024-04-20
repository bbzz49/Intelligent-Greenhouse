#ifndef __INTERACTWITHTCP_H__
#define __INTERACTWITHTCP_H__

void Module_Init(void);
void SendPropertyToTcp(void);
void HandleCommandFromTcp(void);
void SendEventToTcp(char *identifier,uint8_t name);
#endif
