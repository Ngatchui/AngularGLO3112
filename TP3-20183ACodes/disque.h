#ifndef DISQUE_H
#define DISQUE_H
 
// Les fonctions d'acces au disque dur. Les fonctions bd_*
// utilisent ces fonctions pour lire & ecrire sur le disque.
int ReadBlock(UINT16 BlockNum, char *pBuffer);
int WriteBlock(UINT16 BlockNum, const char *pBuffer);
//20183ASE
#endif
