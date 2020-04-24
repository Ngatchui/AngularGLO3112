#include "UFS.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "disque.h"
 
// Quelques fonctions qui pourraient vous être utiles
int NumberofDirEntry(int Size) {
	return Size/sizeof(DirEntry);
}

int min(int a, int b) {
	return a<b ? a : b;
}

int max(int a, int b) {
	return a>b ? a : b;
}

/* Cette fonction va extraire le repertoire d'une chemin d'acces complet, et le copier
   dans pDir.  Par exemple, si le chemin fourni pPath="/doc/tmp/a.txt", cette fonction va
   copier dans pDir le string "/doc/tmp" . Si le chemin fourni est pPath="/a.txt", la fonction
   va retourner pDir="/". Si le string fourni est pPath="/", cette fonction va retourner pDir="/".
   Cette fonction est calquée sur dirname, que je ne conseille pas d'utiliser car elle fait appel
   à des variables statiques/modifie le string entrant. Voir plus bas pour un exemple d'utilisation. */
int GetDirFromPath(const char *pPath, char *pDir) {
	strcpy(pDir,pPath);
	int len = strlen(pDir); // length, EXCLUDING null
	int index;

	// On va a reculons, de la fin au debut
	while (pDir[len]!='/') {
		len--;
		if (len <0) {
			// Il n'y avait pas de slash dans le pathname
			return 0;
		}
	}
	if (len==0) {
		// Le fichier se trouve dans le root!
		pDir[0] = '/';
		pDir[1] = 0;
	}
	else {
		// On remplace le slash par une fin de chaine de caractere
		pDir[len] = '\0';
	}
	return 1;
}

/* Cette fonction va extraire le nom de fichier d'une chemin d'acces complet.
   Par exemple, si le chemin fourni pPath="/doc/tmp/a.txt", cette fonction va
   copier dans pFilename le string "a.txt" . La fonction retourne 1 si elle
   a trouvée le nom de fichier avec succes, et 0 autrement. Voir plus bas pour
   un exemple de son utilisation. */
int GetFilenameFromPath(const char *pPath, char *pFilename) {
	// Pour extraire le nom de fichier d'un path complet
	char *pStrippedFilename = strrchr(pPath,'/');
	if (pStrippedFilename!=NULL) {
		++pStrippedFilename; // On avance pour passer le slash
		if ((*pStrippedFilename) != '\0') {
			// On copie le nom de fichier trouve
			strcpy(pFilename, pStrippedFilename);
			return 1;
		}
	}
	return 0;
}


/* Un exemple d'utilisation des deux fonctions ci-dessus :
int bd_create(const char *pFilename) {
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) return 0;
	GetFilenameFromPath(pFilename, StringFilename);
	                  ...
*/


/* Cette fonction sert à afficher à l'écran le contenu d'une structure d'i-node */
void printiNode(iNodeEntry iNode) {
	printf("\t\t========= inode %d ===========\n",iNode.iNodeStat.st_ino);
	printf("\t\t  blocks:%d\n",iNode.iNodeStat.st_blocks);
	printf("\t\t  size:%d\n",iNode.iNodeStat.st_size);
	printf("\t\t  mode:0x%x\n",iNode.iNodeStat.st_mode);
	int index = 0;
	for (index =0; index < N_BLOCK_PER_INODE; index++) {
		printf("\t\t      Block[%d]=%d\n",index,iNode.Block[index]);
	}
}


// m retourne l inode contenu dans un block valide seulement du block 6 a 23 contenant des inodes
/*
iNodeEntry* recupererInodeRepertoire(UINT16 num,const char *pFilename1){
	
		char BlockCourantInodeEntry[BLOCK_SIZE];
		ReadBlock(num, BlockCourantInodeEntry);
		iNodeEntry *pInode=(iNodeEntry*)BlockCourantInodeEntry;
		printiNode(*pInode);
		//printf("NON\n ",pInode->Block[0]);
		//listerEntreeRepertoireInode(pInode);
		printf("NON\n ");

		
	return pInode;	


}
*/

int entreeRepertoireContientRepertoire(DirEntry *pDirEntry,const char *pFilename){

	int numInode=0;
	int i=0;
	for (i = 0; i<4; i++){

		if (strcmp(pFilename,pDirEntry[i].Filename)==0) {
			return numInode;
			}
		
	}

return -2;
}
int entreeRepertoireContientFichier(DirEntry *pDirEntry,const char *pFilename){

	int numInode=0;
	int i=0;
	for (i = 0; i<6; i++){

		if (strcmp(pFilename,pDirEntry[i].Filename)==0) {
			numInode=pDirEntry[i].iNode;
			return numInode;
			}
		
	}

return -2;
}

iNodeEntry* convertirNumInode(UINT16 num){
	
	char BlockCourantInodeEntry[BLOCK_SIZE];
	
	int n=0;
	//parcourt pour rechercher linode correspondant au numero
	for (n =5; n <23; n++) {	
		ReadBlock(n, BlockCourantInodeEntry);
		iNodeEntry *pInode=(iNodeEntry*)BlockCourantInodeEntry;
		//printiNode(*pInode);
		if (pInode->iNodeStat.st_ino==num) {
			return pInode;
			break;
		}
		
	
	}



}

iNodeEntry* recupererInodeParent(UINT16 num,const char *pFilename){

	iNodeEntry *pInode1;	
	char BlockCourantInodeEntry[BLOCK_SIZE];
	int inodeRecherche=0;
	int n=0;
	//parcourt pour rechercher le nom du fichier dans les repertoires
	for (n =num; n <23; n++) {	
		ReadBlock(n, BlockCourantInodeEntry);
		iNodeEntry *pInode=(iNodeEntry*)BlockCourantInodeEntry;
		if (strcmp(pFilename,"")==0){

			 return pInode;}
			char DataBlockDirEntry[BLOCK_SIZE];
			ReadBlock(pInode->Block[0], DataBlockDirEntry); //
			DirEntry *pDirEntry = (DirEntry *)DataBlockDirEntry; 
			int retourinode= entreeRepertoireContientFichier(pDirEntry,pFilename);
			if (retourinode==-2){ }
			else{  inodeRecherche=retourinode;//numero de node recherche
				 return pInode;
				break;
				}
	
	}


return pInode1;


}
int listerEntreeRepertoireInode(iNodeEntry *pInode) {
	int n=0;
	int nbEntree=0;

		char BlockCourantDirEntry[N_BLOCK_PER_INODE];
		ReadBlock(pInode->Block[0], BlockCourantDirEntry);
		nbEntree =  pInode->iNodeStat.st_size/sizeof(DirEntry);

		DirEntry *pDe=(DirEntry*)BlockCourantDirEntry;
		for (n =0; n <nbEntree ; n++) {	
			if (pDe[n].iNode) {
		
			//printf("Le nie;e fichier du repertoire inode %d avec nom  %s  %d \n ",pDe[n].iNode,pDe[n].Filename,nbEntree);
			//printiNode(*pInode);
			}
		}

	return nbEntree;
	}
	

int repertoireExiste(UINT16 num,const char *pFilename){

	char BlockCourantInodeEntry[BLOCK_SIZE];
	int inodeRecherche=-3;
	int n=0;
	//parcourt pour rechercher le nom du fichier dans les repertoires
	for (n =num; n <23; n++) {	
		ReadBlock(n, BlockCourantInodeEntry);
		iNodeEntry *pInode2=(iNodeEntry*)BlockCourantInodeEntry;
		if (strcmp(pFilename,"/")==0){
			 return 1;}
		//printiNode(*pInode);
		if (pInode2->iNodeStat.st_mode & G_IFDIR) {
			char DataBlockDirEntry[BLOCK_SIZE];
			ReadBlock(pInode2->Block[0], DataBlockDirEntry); //
			DirEntry *pDirEntry2 = (DirEntry *)DataBlockDirEntry; 
			int retourinode= entreeRepertoireContientRepertoire(pDirEntry2,pFilename);
			if (retourinode==-2){ }
			else{  inodeRecherche=retourinode;
				//inodeParent=pInode2;
				break;
				}
		}
		
	
	}
return inodeRecherche;
}


int fichierExiste(UINT16 num,const char *pFilename){

	char BlockCourantInodeEntry[BLOCK_SIZE];
	int inodeRecherche=-3;
	int n=0;
	//parcourt pour rechercher le nom du fichier dans les repertoires
	for (n =num; n <23; n++) {	
		ReadBlock(n, BlockCourantInodeEntry);
		iNodeEntry *pInode2=(iNodeEntry*)BlockCourantInodeEntry;
		if (strcmp(pFilename,"")==0){
			 return 1;}
		//printiNode(*pInode);
		//if (pInode2->iNodeStat.st_mode & G_IFDIR) {
			//printf("BLOCK %d\n ",n);
			//printf("DONNEES DANS %d\n ",pInode2->Block[0]);
			char DataBlockDirEntry[BLOCK_SIZE];
			ReadBlock(pInode2->Block[0], DataBlockDirEntry); //
			DirEntry *pDirEntry2 = (DirEntry *)DataBlockDirEntry; 
			//printf("nom de fichier %s\n",pFilename);
			int retourinode= entreeRepertoireContientFichier(pDirEntry2,pFilename);
			if (retourinode==-2){ }
			else {  inodeRecherche=retourinode;
				
				break;
				}
		//}
		
	
	}
return inodeRecherche;
}





iNodeEntry* recupererInode(UINT16 num,const char *pFilename){

	iNodeEntry *pInode1;	
	char BlockCourantInodeEntry[BLOCK_SIZE];
	int inodeRecherche=0;
	int n=0;
	//parcourt pour rechercher le nom du fichier dans les repertoires
	for (n =num; n <23; n++) {	
		ReadBlock(n, BlockCourantInodeEntry);
		iNodeEntry *pInode=(iNodeEntry*)BlockCourantInodeEntry;
		if (strcmp(pFilename,"")==0){
			 return pInode;}
			char DataBlockDirEntry[BLOCK_SIZE];
			ReadBlock(pInode->Block[0], DataBlockDirEntry); //
			DirEntry *pDirEntry = (DirEntry *)DataBlockDirEntry; 
			int retourinode= entreeRepertoireContientFichier(pDirEntry,pFilename);
			if (retourinode==-2){ }
			else{  inodeRecherche=retourinode;//numero de node recherche
				return convertirNumInode(inodeRecherche);
				break;
				}
	
	}


return pInode1;


}

int ReleaseFreeBlock(UINT16 Blocknum) {
	char DataBlockInodeEntry1[BLOCK_SIZE];
 	ReadBlock(FREE_BLOCK_BITMAP, DataBlockInodeEntry1);
	printf("GLOFS: relache block %d\n ",Blocknum );
 	DataBlockInodeEntry1[Blocknum] = 1;

 	WriteBlock(FREE_BLOCK_BITMAP, DataBlockInodeEntry1);//Declare bloc libre
	return 1;
}


int bd_readdir(const char *pDirLocation, DirEntry **ppListeFichiers) {

	iNodeEntry *inodeParent;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pDirLocation, StringDir)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pDirLocation, StringFilename);
	if((strcmp("/",StringDir)==0)&&(strlen(pDirLocation)==1)){
		iNodeEntry *inodeR = recupererInode(numblock,"");
		char DataBlockDirEntryr[BLOCK_SIZE];
		ReadBlock(inodeR->Block[0], DataBlockDirEntryr); 
		DirEntry * pDirEntryr = (DirEntry *)DataBlockDirEntryr; 
		(*ppListeFichiers) = (DirEntry*) malloc(inodeR->iNodeStat.st_size);
		int nbEntree =  inodeR->iNodeStat.st_size/sizeof(DirEntry);	
		memcpy(*ppListeFichiers, pDirEntryr,nbEntree *sizeof(DirEntry));
		return nbEntree;	
		}
	int exister= fichierExiste(numblock,StringFilename);
	if(exister == -3){

			return -1;
		}
	
 	iNodeEntry *inodeR = recupererInode(numblock,StringFilename);
 	if((inodeR->iNodeStat.st_mode & G_IFREG)){
		
 		return -1;
 	}
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(inodeR->Block[0], DataBlockDirEntry); 
	DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	(*ppListeFichiers) = (DirEntry*) malloc(inodeR->iNodeStat.st_size);
	int nbEntree =  inodeR->iNodeStat.st_size/sizeof(DirEntry);	
	memcpy(*ppListeFichiers, pDirEntry,nbEntree *sizeof(DirEntry));
	return nbEntree;

	
}

int bd_chmod(const char *pFilename, UINT16 st_mode) {


	iNodeEntry *inodeParent;
	int reponse=0;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) reponse=-1;
	GetFilenameFromPath(pFilename, StringFilename);

	int existe= fichierExiste(numblock,StringFilename);
	if(existe == -3){
			
			return -1;
		}



	iNodeEntry  *inode = recupererInode(numblock,StringFilename);

	inode->iNodeStat.st_mode|=G_IRWXU |G_IRWXG |st_mode;




	return 0;

	}




//afficher la structure d'un fichier/repertoir

int bd_stat(const char *pFilename, gstat *pStat) {
	
	iNodeEntry *inodeParent;
	int reponse=0;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) return-1;
	GetFilenameFromPath(pFilename, StringFilename);		
	int existe= fichierExiste(numblock,StringFilename);
	if(existe == -3){
			return -1;
		}
	const iNodeEntry  *inode = recupererInode(numblock,StringFilename);
	*pStat = inode->iNodeStat;
	return 0;
	

}


int bd_countfreeblocks(void) {
	
	int count = 0;
	int index = 0;
	char BlocFreeBitmap[BLOCK_SIZE];
 	ReadBlock(FREE_BLOCK_BITMAP, BlocFreeBitmap);
	
	for (index =0; index < BLOCK_SIZE; index++) {
		if (BlocFreeBitmap[index] != 0) 
			count++;
		
	}
	return count;
}

int bd_create(const char *pFilename) {
	int reponse=0;
	iNodeEntry *inodeParent;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) return-1;
	GetFilenameFromPath(pFilename, StringFilename);
	int existerf= fichierExiste(numblock,StringFilename);
	if(existerf == -3){
		}
	else{ 
			return -1;
		}

//if (GetDirFromPath(pFilename,StringFilename2)==0) return-1;
	int exister= repertoireExiste(numblock,StringDir);
	if(exister == -3){
			return -1;
		}

//iNodeEntry *inode = recupererInode(numblock,StringDir);
 	//iNodeEntry *inode = recupererInodeRepertoire(numblock,StringDir);
if((strcmp("/",StringDir)==0)) inodeParent=recupererInodeParent(numblock,"");
	else	inodeParent=recupererInodeParent(numblock,StringFilename);
int nbEntree =inodeParent->iNodeStat.st_size/sizeof(DirEntry);

	//int nEntree=listerEntreeRepertoireInode(inode);
	int i=0;
 
	inodeParent->iNodeStat.st_size +=1;
	inodeParent->iNodeStat.st_nlink +=1;
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
	DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	//int nbEntree =listerEntreeRepertoireInode(inode);
	ino nouvIno;
	strcpy(pDirEntry[nbEntree].Filename,StringFilename);
	pDirEntry[nbEntree].iNode=nouvIno;	
	iNodeEntry pInodenew;
/*
	pInodenew->iNodeStat.st_ino =nouvIno;
	pInodenew->iNodeStat.st_nlink =1;
	pInodenew->iNodeStat.st_mode |=G_IFREG;
	pInodenew->iNodeStat.st_mode  |=G_IRWXU |G_IRWXG;
	WriteBlock(inode->Block[0], DataBlockInodeEntry);
*/	
	char DataBlockInodeEntry1[BLOCK_SIZE];
 	ReadBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);
	printf("GLOFS: Saisie inode %d ===========\n ",nouvIno);
 	DataBlockInodeEntry1[nouvIno] = 0;
 	WriteBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);//Declqre inode occupe

	return 1;
}

int bd_unlink(const char *pFilename) {

	iNodeEntry *inodeParent;
	int reponse=0;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) return-1;
	GetFilenameFromPath(pFilename, StringFilename);

	int existe= fichierExiste(numblock,StringFilename);
	if(existe == -3){
			return -1;
		}
	iNodeEntry *inode = recupererInode(numblock,StringFilename);

 	if(!(inode->iNodeStat.st_mode & G_IFREG)){
 		return -2;
 	}
 	int j=0;
	int i=0;
if((strcmp("/",StringDir)==0)) inodeParent=recupererInodeParent(numblock,"");
	else	inodeParent=recupererInodeParent(numblock,StringFilename);
int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);
	//inodeParent->iNodeStat.st_size -=1;
	inodeParent->iNodeStat.st_nlink -=1;
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
	DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
    	DirEntry  pDirEntry1;
	//int nbEntreeParent =listerEntreeRepertoireInode(inodeParent);//effacer entree parent
	for (i = 2; i<nbEntreeParent; i++){
		if(strcmp("/",pDirEntry[i].Filename)==0){
			pDirEntry[i] = pDirEntry1;
			j=i;
			break;
		}
	}

//compactage
	for (i = j; i<nbEntreeParent; i++){
			pDirEntry[i] = pDirEntry[i+1];
			
	}

//liberer les blosc de donnees du fichier
	for (i = 1; i<14; i++){

			//printf("relache block%d ===========\n ",inode->Block[i]);
			ReleaseFreeBlock(inode->Block[i]);
			
	}
//liberer inode

 	char DataBlockInodeEntry1[BLOCK_SIZE];
 	ReadBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);
	printf("GLOFS: Relache inode %d ===========\n ",inode->iNodeStat.st_ino);
 	//DataBlockInodeEntry1[inode->iNodeStat.st_ino] = 1;
 	//WriteBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);

	return 1;
}


int bd_rmdir(const char *pFilename) {
	iNodeEntry *inodeParent;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pFilename, StringDir)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pFilename, StringFilename);
	int exister= repertoireExiste(numblock,StringFilename);
	if(exister == -3){
			//printf("REPERTOIRE EXISTE PAS\n");
			return -1;
		}
	//else 	printf("REPERTOIRE EXISTE BIEN\n");
	
 	iNodeEntry *inodeR = recupererInode(numblock,StringFilename);

//printf("GLOFS: Q RELQCHER BLOC %d ===========\n ",inodeR->Block[0]);
 	if((inodeR->iNodeStat.st_mode & G_IFREG)){
 		
 		return -2;
 	}
 	
	int nEntree=inodeR->iNodeStat.st_size/sizeof(DirEntry);
	if(nEntree>2) return -3;
	 ReleaseFreeBlock(inodeR->Block[0]);
	int i=0;
 	
 	char DataBlockInodeEntry1[BLOCK_SIZE];
 	ReadBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);
	printf("GLOFS: Relache inode %d \n ",inodeR->iNodeStat.st_ino);
 	DataBlockInodeEntry1[inodeR->iNodeStat.st_ino] = 1;

 	WriteBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);//Declqre inode libre
if((strcmp("/",StringDir)==0)) inodeParent=recupererInodeParent(numblock,"");
	else	inodeParent=recupererInodeParent(numblock,StringFilename);

int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);
	inodeParent->iNodeStat.st_size -=1;
	inodeParent->iNodeStat.st_nlink -=1;
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
	DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	//char *nomDuRepertoir= strrchr(pFilename, '/');
    	//nomDuRepertoir += 1;
    	DirEntry  pDirEntry1;
int k=0;
	//int nbEntreeParent =listerEntreeRepertoireInode(inodeR);//effacer entree parent
	for (i = 2; i<nbEntreeParent; i++){
		if(strcmp(StringFilename,pDirEntry[i].Filename)==0){

			pDirEntry[i] = pDirEntry1;
			if(i!=nbEntreeParent-1){

			 k=i;

		//compactage
			for (i = k; i<nbEntreeParent; i++){
					pDirEntry[i] = pDirEntry[i+1];
			
			}
			}
			break;
		}
	}
	printf("GLOFS: Saisie Block  %d \n ",inodeParent->Block[0]);
	WriteBlock(inodeParent->Block[0], DataBlockDirEntry);

	return 1;
}

int bd_rename(const char *pFilename, const char *pDestFilename) {
	

iNodeEntry *inodeParent;
iNodeEntry *inodeParent2;
	int numblock=5;
	char StringDir1[256];
	char StringFilename1[256];
	char StringDir2[256];
	char StringFilename2[256];
	if (GetDirFromPath(pFilename, StringDir1)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pFilename, StringFilename1);
	if (GetDirFromPath(pDestFilename, StringDir2)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pDestFilename, StringFilename2);
	//printf("nom repertoire = %s \n",StringDir);
	//printf("nom fichier = %s \n",StringFilename);
	//if((strcmp(StringDir1,StringDir2)==0))

	int exister= fichierExiste(numblock,StringFilename1);
	if(exister == -3){
			return -1;
		}
char StringFilename3[256];

GetFilenameFromPath(StringDir2, StringFilename3);
	int existerd= fichierExiste(numblock,StringDir2);
	if(existerd == -3){
		}
	else {
		return -2;
		}

//iNodeEntry *inodeS = recupererInodeRepertoire(numblock,StringDir1);
 	iNodeEntry *inodeS = recupererInode(numblock,StringFilename1);

 	if((inodeS->iNodeStat.st_mode & G_IFREG)){
 		
 		
 	}
 	else{
	//int nEntree=inodeParent->iNodeStat.st_size/sizeof(DirEntry);
	//if(nEntree>2) return -3;
	}

	
	if((strcmp(StringDir1,StringDir2)==0)){	//C est un renommage

		int i=0;
	 	if((strcmp("/",StringDir1)==0)) inodeParent=recupererInodeParent(numblock,"");
		else	inodeParent=recupererInodeParent(numblock,StringFilename1);
		int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);
		char DataBlockDirEntry[BLOCK_SIZE];
		ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
		DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	
	    
		//modifier entree parent
		for (i = 0; i<nbEntreeParent; i++){
			if(strcmp( StringFilename1,pDirEntry[i].Filename)==0){
				strcpy(pDirEntry[i].Filename,StringFilename2);
				break;
			}
		}

		printf("GLOFS: Saisie Block  %d \n ",inodeParent->Block[0]);
		WriteBlock(inodeParent->Block[0], DataBlockDirEntry);


	}else{ //c est un deplacement
		//recherche du parent destination

		iNodeEntry *inodeD = recupererInode(numblock,StringFilename1);
//iNodeEntry *inodeD = recupererInoderepertoire(numblock,StringDir1);
	
	 	if((inodeD->iNodeStat.st_mode & G_IFREG)){
	 		//printf("Inode de fichier\n");
	 	
	 	}
	 	else{
		int nEntreeD=inodeD->iNodeStat.st_size/sizeof(DirEntry);
		//if(nEntree>2) return -3;
		}

		if((strcmp("/",StringDir2)==0)) inodeParent2=recupererInodeParent(numblock,"");
			else	inodeParent2=recupererInodeParent(numblock,StringFilename2);
		int nbEntreeParent2 =inodeParent2->iNodeStat.st_size/sizeof(DirEntry);
		char DataBlockDirEntry2[BLOCK_SIZE];
		ReadBlock(inodeParent2->Block[0], DataBlockDirEntry2); 
		DirEntry * pDirEntry2 = (DirEntry *)DataBlockDirEntry2; 
		    
		//int nbEntreeParent2 =listerEntreeRepertoireInode(inodeParent2);
		//RECHERCHER DU CONTENU DU  BLOC DU PQRENT SOURCE
	

		int i=0;
	 	
		char DataBlockDirEntry[BLOCK_SIZE];
		ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
		DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	
	    
		if((strcmp("/",StringDir1)==0)) inodeParent=recupererInodeParent(numblock,"");
		else	inodeParent=recupererInodeParent(numblock,StringFilename1);
		int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);//modifier entree parent
		for (i = 0; i<nbEntreeParent; i++){
			if(strcmp( StringFilename1,pDirEntry[i].Filename)==0){
				//on copie le contenu de entre de lq source a lq fin de entre du parent dest
				strcpy(pDirEntry2[nbEntreeParent2].Filename,pDirEntry[i].Filename);
				pDirEntry2[nbEntreeParent2].iNode=pDirEntry[i].iNode;
				//vider le contenu de l entre de la source
				ino inodeAliberer=pDirEntry[i].iNode;
				ino nouvIno;
				strcpy(pDirEntry[i].Filename,"");
				pDirEntry[i].iNode=nouvIno;
				//liberer l inode

				
				break;
			}
		}
		printf("GLOFS: Saisie block %d ===========\n ",inodeParent2->Block[0]);
		WriteBlock(inodeParent2->Block[0], DataBlockDirEntry2);//enregistrement
		printf("GLOFS: Saisie block %d ===========\n ",inodeParent->Block[0]);
		WriteBlock(inodeParent->Block[0], DataBlockDirEntry);//enregistrement
//modificqtion de stlink des pqrents
		printf("GLOFS: Saisie inode %d ===========\n ",inodeParent->iNodeStat.st_ino);
		inodeParent->iNodeStat.st_size -=1;
		inodeParent->iNodeStat.st_nlink -=1;
		inodeParent2->iNodeStat.st_size +=1;
		inodeParent2->iNodeStat.st_nlink +=1;
	
/*		char DataBlockInodeEntree[BLOCK_SIZE];
	 	ReadBlock(FREE_INODE_BITMAP, DataBlockInodeEntree);
		printf("relache inode %d ===========\n ",inodeS->iNodeStat.st_ino);
	 	DataBlockInodeEntree[inodeS->iNodeStat.st_ino] = 1;

	 	WriteBlock(FREE_INODE_BITMAP, DataBlockInodeEntree);//Declqre inode libre
*/
	}

	return 1;
}
	
int bd_hardlink(const char *pPathExistant, const char *pPathNouveauLien) {

iNodeEntry *inodeParentFichS;
iNodeEntry *inodeParent;
iNodeEntry *inodeParent2;
	int numblock=5;
	char StringDir1[256];
	char StringFilename1[256];
	char StringDir2[256];
	char StringFilename2[256];
	if (GetDirFromPath(pPathExistant, StringDir1)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pPathExistant, StringFilename1);
	if (GetDirFromPath(pPathNouveauLien, StringDir2)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pPathNouveauLien, StringFilename2);

	int exister= fichierExiste(numblock,StringFilename2);
	if(exister == -3){
		}
	else {
		return -2;
		}

	int existeS= fichierExiste(numblock,StringFilename1);
	if(existeS == -3){
			return-1;
		}
 	
	inodeParentFichS=recupererInode(numblock,StringFilename1);
 	if((inodeParentFichS->iNodeStat.st_mode & G_IFREG)){	
 	}
 	else{
	return -3;	
	}

	if((strcmp("/",StringDir1)==0)) inodeParent=recupererInodeParent(numblock,"");
	else	inodeParent=recupererInodeParent(numblock,StringFilename1);

	if((strcmp("/",StringDir2)==0)) inodeParent2=recupererInodeParent(numblock,"");
	else	inodeParent2=recupererInodeParent(numblock,StringFilename2);

	char DataBlockDirEntry2[BLOCK_SIZE];
		ReadBlock(inodeParent2->Block[0], DataBlockDirEntry2); 
		DirEntry * pDirEntry2 = (DirEntry *)DataBlockDirEntry2; 
		int nbEntreeParent2 =inodeParent2->iNodeStat.st_size/sizeof(DirEntry);
		//RECHERCHER DU CONTENU DU  BLOC DU PQRENT SOURCE
		int i=0;	
		char DataBlockDirEntry[BLOCK_SIZE];
		ReadBlock(inodeParent->Block[0], DataBlockDirEntry);
		DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
		int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);	
		for (i = 0; i<nbEntreeParent; i++){
			if(strcmp( StringFilename1,pDirEntry[i].Filename)==0){
				//on copie le contenu de entre de lq source a lq fin de entre du parent dest			
				strcpy(pDirEntry2[nbEntreeParent2].Filename,pDirEntry[i].Filename);
				pDirEntry2[nbEntreeParent2].iNode=pDirEntry[i].iNode;
				//modificqtion de stlink du pqrent dest
		inodeParent2->iNodeStat.st_size +=1;
		inodeParent2->iNodeStat.st_nlink +=1;
		inodeParent->iNodeStat.st_nlink +=1;
				break;
			}
		}

		WriteBlock(inodeParent2->Block[0], DataBlockDirEntry2);//enregistrement


	return 1;
}

int bd_mkdir(const char *pDirName) {

	iNodeEntry *inodeParent;
	int numblock=5;
	char StringDir[256];
	char StringFilename[256];
	if (GetDirFromPath(pDirName, StringDir)==0) return -1;//Non trouve le bon nom
	GetFilenameFromPath(pDirName, StringFilename);
//	printf("nom repertoire = %s \n",StringDir);
//	printf("nom fichier = %s \n",StringFilename);
	int exister= repertoireExiste(numblock,StringDir);
	if(exister == -3){
			
		}
	else {
		return -2;
		}

char StringFilename3[256];
GetFilenameFromPath( StringDir, StringFilename3);
 	if((strcmp("/",StringDir)==0)) inodeParent=recupererInodeParent(numblock,"");
	else	inodeParent=recupererInodeParent(numblock,StringFilename3);

 	if((inodeParent->iNodeStat.st_mode & G_IFREG)){
 	
 		return -2;
 	}
 	
   int nbEntreeParent =inodeParent->iNodeStat.st_size/sizeof(DirEntry);
	if(nbEntreeParent<=2) return -3;

	int i=0;
 	ino newIno;
/*
	iNodeEntry pInodenew;
	pInodenew->iNodeStat.st_ino =newIno;
	pInodenew->iNodeStat.st_nlink =1;
	pInodenew->iNodeStat.st_mode |=G_IFREG;
	pInodenew->iNodeStat.st_mode  |=G_IRWXU |G_IRWXG;
*/
 	char DataBlockInodeEntry1[BLOCK_SIZE];
 	ReadBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);
	printf("GLOFS: Saisie inode %d ===========\n ",newIno);
 	DataBlockInodeEntry1[newIno] = 0;
	
 	WriteBlock(FREE_INODE_BITMAP, DataBlockInodeEntry1);//Declqre inode occupe


	inodeParent->iNodeStat.st_size +=1;
	inodeParent->iNodeStat.st_nlink +=1;
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(inodeParent->Block[0], DataBlockDirEntry); 
	DirEntry * pDirEntry = (DirEntry *)DataBlockDirEntry; 
	//char *nomDuRepertoir= strrchr(pFilename, '/');
    	//nomDuRepertoir += 1;
 
	//int nbEntreeParent =listerEntreeRepertoireInode(inodeR);//modifier entree parent
	strcpy(pDirEntry[nbEntreeParent].Filename,StringFilename);
	pDirEntry[nbEntreeParent].iNode=newIno;
		
	WriteBlock(inodeParent->Block[0], DataBlockDirEntry);
	
	char DataBlockDirEntry2[BLOCK_SIZE];//ecrit nouveau
	ReadBlock(100, DataBlockDirEntry2); 

	DirEntry * pDirEntry1 = (DirEntry *)DataBlockDirEntry; 
	strcpy(pDirEntry1[2].Filename,StringFilename);
	pDirEntry1[2].iNode=newIno;
	strcpy(pDirEntry1[0].Filename,".");
	pDirEntry1[0].iNode=inodeParent->iNodeStat.st_ino;
	strcpy(pDirEntry1[1].Filename,"..");
	pDirEntry1[1].iNode=inodeParent->iNodeStat.st_ino;
	printf("GLOFS: Saisie block %d ===========\n ",100);

	WriteBlock(100, DataBlockDirEntry2);

	

	return 1;
}
//20183ASE
