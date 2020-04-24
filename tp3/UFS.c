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
   Cette fonction est calquée sur dirname(), que je ne conseille pas d'utiliser car elle fait appel
   à des variables statiques/modifie le string entrant. Voir plus bas pour un exemple d'utilisation. 
   3e6a98197487be5b26d0e4ec2051411f
*/
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
   un exemple d'utilisation. */   
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


/* ----------------------------------------------------------------------------------------
	C'est votre partie, bon succès!
	3e6a98197487be5b26d0e4ec2051411f
   ---------------------------------------------------------------------------------------- */

iNodeEntry *chercherInode(ino *unInode, char blocksD[BLOCK_SIZE]){

	ReadBlock(FREE_INODE_BITMAP,blocksD);
	UINT16 numBlockInode = BASE_BLOCK_INODE + (*unInode/NUM_INODE_PER_BLOCK);
	UINT16 offset = (*unInode%NUM_INODE_PER_BLOCK);
	ReadBlock(numBlockInode,blocksD);
	iNodeEntry *NodeEntry = (iNodeEntry*)blocksD;
	return (NodeEntry + offset);

}

int trouverInodeFichier(const char *nomFichier, ino *inodeCourant, ino *inodeParent, const char *pfichierTemp){
	int i;
	int nbreTotalDir;
	char DataBlockInodeEntry[BLOCK_SIZE];
	iNodeEntry *NodeEntry = chercherInode(inodeCourant,DataBlockInodeEntry);
	if(NodeEntry->iNodeStat.st_mode & G_IFDIR){
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(NodeEntry->Block[0],DataBlockDirEntry);
	DirEntry *phDirEntry = (DirEntry*)DataBlockDirEntry;
	nbreTotalDir = NumberofDirEntry(BLOCK_SIZE);	 
	
	for(i = 0; i<nbreTotalDir; i++ ){
	*inodeParent = *inodeCourant;
	     if(strcmp(pfichierTemp,phDirEntry[i].Filename)==0){
		*inodeCourant = phDirEntry[i].iNode;
		pfichierTemp = strtok(NULL, "/");
		if(pfichierTemp !=NULL){
		
		return trouverInodeFichier(nomFichier,inodeCourant,inodeParent,pfichierTemp);
	}else{
		return 0;
	}
}
}
if(strcmp(pfichierTemp,phDirEntry[i].Filename)==0){
	printf("fichier ou repetoire introuvable");
	return -2;
	}
	
}
return -1;
	
}


int getInodeNomFichier(ino *inodeCourant,ino *inodeParent,const char* pUnfichier){
	*inodeParent =ROOT_INODE;
	*inodeCourant =ROOT_INODE;

	if(strcmp(pUnfichier,"/")==0){
	  return 0;
	}
	char pPath[256];
	char *pfichierTemp;
	strcpy(pPath,pUnfichier);
	pfichierTemp = strtok(pPath,"/");
	char * nomFichier = strrchr(pUnfichier,'/');
	nomFichier +=1;
	return trouverInodeFichier(nomFichier,inodeCourant,inodeParent,pfichierTemp);
}

int trouverUnNumInodeLibre(ino* unInode, char BlockFreeBitMap[BLOCK_SIZE]){

	ReadBlock(FREE_BLOCK_BITMAP,BlockFreeBitMap);
	int libre =0;
	int i;
	  for(i=1; i<N_INODE_ON_DISK; i++)
	    if(BlockFreeBitMap[i] !=0){
		  *unInode =i;
		   printf("GLOFS: Saisie i-node %d\n",*unInode);
		   libre =1;
	   	   BlockFreeBitMap[i]=0;
		   WriteBlock(FREE_BLOCK_BITMAP,BlockFreeBitMap);
		   break;
		}
		if(libre =0){
		 printf("un inode ne peut plus etre octoyer");
		 return -1;
		}
	return 0;	
}

int EcrireRepEntry(ino*inodeParent, ino*iNodeRep, const char* pFilename, int siRep){
	char DataBlockRepEntree[BLOCK_SIZE];
	char DataBlockInodeEntry[BLOCK_SIZE];

	iNodeEntry *unInode = chercherInode(inodeParent,DataBlockInodeEntry);
	if(siRep){
	   unInode->iNodeStat.st_nlink +=1;
	}
	ReadBlock(unInode->Block[0] ,DataBlockRepEntree);
	DirEntry *phDirEntry = (DirEntry*)DataBlockRepEntree;
	int nbreRepOccuper = unInode->iNodeStat.st_size/sizeof(phDirEntry);
	int nbreMaxRep = NumberofDirEntry(BLOCK_SIZE);
	if(nbreRepOccuper >= nbreMaxRep){
		return -1;
	}
	phDirEntry[nbreRepOccuper].iNode = *iNodeRep;
	strcpy(phDirEntry[nbreRepOccuper].Filename,pFilename);
	unInode->iNodeStat.st_size += sizeof(phDirEntry);
	UINT16 numBlockInode = BASE_BLOCK_INODE + (*inodeParent/NUM_INODE_PER_BLOCK);
	WriteBlock(BASE_BLOCK_INODE + *inodeParent, (char*)unInode);
	WriteBlock(unInode->Block[0], (char*)phDirEntry);
	
	return 0;

}
int verifNomFichierDansRep(char *pFilename,DirEntry*repEntree){
	if(strcmp(pFilename,repEntree->Filename)==0){
		return 0;
		}
 	return strcmp(pFilename,repEntree->Filename);
	
}

int ecrireInodedansBlock(ino *inodeNum, int siRep){

	char DataBlockEntree1[BLOCK_SIZE];
	UINT16 prochainNumBlockLibre;
	char DataBlockEntree2[BLOCK_SIZE];
	char DataBlockEntree3[BLOCK_SIZE];

	int obtenirInode = trouverUnNumInodeLibre(inodeNum,DataBlockEntree2);
	if(obtenirInode !=0){
		return -1;
		}

	iNodeEntry *unInode = chercherInode(inodeNum,DataBlockEntree1);

	if(siRep){
	     obtenirInode = trouverUnNumInodeLibre(&prochainNumBlockLibre,DataBlockEntree3);
		if(obtenirInode !=0){
		return -1;
		}
	          unInode->iNodeStat.st_mode |= G_IFDIR;
		  unInode->iNodeStat.st_mode &= ~G_IFDIR;
	          unInode->Block[0] = prochainNumBlockLibre;
	}
	else {
		unInode->iNodeStat.st_mode |= G_IFDIR;
		unInode->iNodeStat.st_mode &= ~G_IFDIR;
	}
	unInode->iNodeStat.st_ino = *inodeNum;
	unInode->iNodeStat.st_nlink =1;
	unInode->iNodeStat.st_size = 0;
	unInode->iNodeStat.st_blocks = 0;

	WriteBlock(prochainNumBlockLibre, DataBlockEntree1);
	WriteBlock(BASE_BLOCK_INODE + *inodeNum, DataBlockEntree1);
	

    return 0;
}

int ReleaseFreeBlock(UINT16 BlockNum){
	char BlockFreeBitmap[BLOCK_SIZE];
	ReadBlock(FREE_BLOCK_BITMAP,BlockFreeBitmap);
	BlockFreeBitmap[BlockNum] =1;
	printf("Relache block %d\n",BlockNum);
	WriteBlock(FREE_BLOCK_BITMAP, BlockFreeBitmap);
}

int allouerUnBlock(){
	char inodeFreeBitmap[BLOCK_SIZE];
	ReadBlock(FREE_BLOCK_BITMAP,inodeFreeBitmap);
	int i;
	for(i=0; N_BLOCK_ON_DISK; i++){
		if(inodeFreeBitmap[i]==1){
			  inodeFreeBitmap[i]=0;
			  printf("GLOFS: Saisie block %d\n",i);
			  WriteBlock(FREE_BLOCK_BITMAP, inodeFreeBitmap);
			  return i;
			}
		}
	return 0;
}

void miseAjourInodeDansDisque(char* blockDonnee, iNodeEntry*inode){
	
	UINT16 numBlock = BASE_BLOCK_INODE + (inode->iNodeStat.st_ino/NUM_INODE_PER_BLOCK);
	UINT16 positionInode = inode->iNodeStat.st_ino%NUM_INODE_PER_BLOCK;
	ReadBlock(numBlock,blockDonnee);
	iNodeEntry*unInode = (iNodeEntry*)blockDonnee;
	unInode += (positionInode/sizeof(iNodeEntry));
	memcpy(unInode,inode,sizeof(iNodeEntry));
	WriteBlock(numBlock, blockDonnee);
}

void miseAjourRepDansDisque(char* blockDonnee, DirEntry*unRep){

	UINT16 numBlock = BASE_BLOCK_INODE + (unRep->iNode/NUM_INODE_PER_BLOCK);
	UINT16 positionInode = unRep->iNode%NUM_INODE_PER_BLOCK;
	ReadBlock(numBlock,blockDonnee);
	DirEntry*unRep2 = (DirEntry*)blockDonnee;

	unRep2 += (positionInode/sizeof(DirEntry));
	memcpy(unRep2,unRep,sizeof(DirEntry));
	WriteBlock(numBlock, blockDonnee);
}

int miseAjourInodeDisque(iNodeEntry*pInode){
	char blockDonnee[BLOCK_SIZE];
	const size_t blockInone =BASE_BLOCK_INODE + (pInode->iNodeStat.st_ino/NUM_INODE_PER_BLOCK);

	ReadBlock(blockInone,blockDonnee);
	iNodeEntry*unInodeTemp = (iNodeEntry*)blockDonnee + (pInode->iNodeStat.st_ino%NUM_INODE_PER_BLOCK);

	printf("L inode vn vers mise a jour %d de stat voir \n", pInode->iNodeStat.st_ino);
	memcpy(unInodeTemp,pInode,(sizeof(*unInodeTemp)));

	WriteBlock(blockInone, blockDonnee);
}

int ajoutInodeDansInode(char *filename,iNodeEntry*InodeSce, iNodeEntry*InodeDs ){
	if(!InodeDs->iNodeStat.st_mode & G_IFDIR){
		return -1;
		}
	char datablock[BLOCK_SIZE];
	ReadBlock(InodeDs->Block[0],datablock);
	DirEntry *UnRepertoire = (DirEntry*)datablock;
	if(UnRepertoire==NULL){return -1;}
	int pDir = NumberofDirEntry(InodeDs->iNodeStat.st_size);
	UnRepertoire[pDir].iNode = InodeSce->iNodeStat.st_ino;
	strcpy(UnRepertoire[pDir].Filename,filename);
	WriteBlock(InodeDs->Block[0],datablock);
	InodeDs->iNodeStat.st_size += sizeof(DirEntry);
	return miseAjourInodeDisque(InodeDs);
}		 

void retirerInode(ino unInode){
	char unInodeFreeBitmap[BLOCK_SIZE];
	ReadBlock(FREE_INODE_BITMAP,unInodeFreeBitmap);
	unInodeFreeBitmap[unInode] =1;
	printf("Relache Inode %d\n",unInode);
	WriteBlock(FREE_INODE_BITMAP,unInodeFreeBitmap);

}

int bd_countusedblocks(void) {

	char BlockFreeBitmap[BLOCK_SIZE];
	int cpteur =0;
	int ind;
	ReadBlock(FREE_BLOCK_BITMAP,BlockFreeBitmap);

	for(ind=0;ind <N_BLOCK_ON_DISK; ind++){
		if(BlockFreeBitmap[ind] ==0){
			cpteur++;
		}
	}
	
	return cpteur;
}

int bd_stat(const char *pFilename, gstat *pStat) {

	ino NumInode;
	ino inodeParent;

	int resulInoFichier = getInodeNomFichier(&NumInode,&inodeParent,pFilename);
	if(resulInoFichier !=0){
		return -1;
		}
	char DataBlockInodeEntry[BLOCK_SIZE];
	iNodeEntry *unInode = chercherInode(&NumInode,DataBlockInodeEntry);
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(unInode->Block[0],DataBlockDirEntry);

	pStat->st_ino = unInode->iNodeStat.st_ino;
	pStat->st_mode = unInode->iNodeStat.st_mode;
	pStat->st_nlink = unInode->iNodeStat.st_nlink;
	pStat->st_size = unInode->iNodeStat.st_size;
	pStat->st_blocks = unInode->iNodeStat.st_blocks;

	return 0;
}

int bd_create(const char *pFilename) {
	ino numInode;
	ino inodeParent;
	char nomFichier[256];
	char unRepertoire[256];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
		 return -1;
		}

	char DataBlockInodeEntry[BLOCK_SIZE];
	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);

	char block[BLOCK_SIZE];
	ReadBlock(unInode->Block[0],block);
	DirEntry *repDeBase;
	repDeBase = (DirEntry*) block;
	int nbreRepertoire = unInode->iNodeStat.st_size/sizeof(DirEntry);
	for(int i =0; i< nbreRepertoire; i++){
	    DirEntry *dirEntry = repDeBase + i;
	    int compResultat = strcmp(pFilename,dirEntry->Filename);
	    if(compResultat == 1){
		return -2;
		}
	}

	char blockRepertoire[BLOCK_SIZE];
	ReadBlock(inodeParent,blockRepertoire);
	memcpy(DataBlockInodeEntry,blockRepertoire, BLOCK_SIZE );

	DirEntry repertoire;

	int offset = unInode->iNodeStat.st_size;
	if (offset > BLOCK_SIZE){
		printf("Il y a plus d espace dans le repertoire \n");
		}

	return -1;
}


int bd_read(const char *pFilename, char *buffer, int offset, int numbytes) {

	ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;
	//ino inodeNouv;

	char DataBlockInodeEntry[BLOCK_SIZE];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
		printf("Le fichier %s est inexistant \n", pFilename);
		 return -1;
		}

	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);
	if(unInode->iNodeStat.st_mode & G_IFDIR){
		printf("Le fichier %s est un repertoire \n", pFilename);
		return -2;
		}

	if(unInode->iNodeStat.st_size < offset){
		return 0;
	}

	char DataBlock[BLOCK_SIZE];
	//ReadBlock(unInode->Block[0],DataBlock);
	int n=offset/BLOCK_SIZE;
	int offset_block =offset%BLOCK_SIZE;

	char *donneOffset = DataBlock;
	donneOffset =DataBlock + offset_block;
	int octetctALire = min(unInode->iNodeStat.st_size-offset,numbytes);


	int nombreBlocks = octetctALire/BLOCK_SIZE;

	  if(octetctALire%BLOCK_SIZE >0){

		 nombreBlocks +=1;
		} 

	 int x =BLOCK_SIZE-offset_block;

	for(int j =n; j <= nombreBlocks; j++ ){
		ReadBlock(unInode->Block[j],DataBlock);
		printf("octet a lire %d de stat  \n", octetctALire);
		if(j==n){
		 	memcpy(buffer,donneOffset, x);
		}else{
			if(x<=octetctALire){
			memcpy(buffer+x,DataBlock, 256 );
		printf("octet lu dans buffer %d de stat   \n", x);
			}
			x+=256;
	printf("octet lu %d de stat   \n", x);
		}
	}

	return octetctALire;
}


int bd_write(const char *pFilename, const char *buffer, int offset, int numbytes) { 
	ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;
	//ino inodeNouv;

	char DataBlockInodeEntry[BLOCK_SIZE];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
		printf("Le fichier %s est inexistant d\n", pFilename);
		return -1;
		}

	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);

	printf("taille offset %d\n", offset);

	if(unInode->iNodeStat.st_mode & G_IFDIR ){
		printf("Le fichier %s est un repertoire \n", pFilename);
		return -2;
		}

	if(offset > unInode->iNodeStat.st_size){
		printf("L'offset demande est trop grand! \n");
		return -3;
		}

	if(offset >= DISKSIZE){

		return -4;
}


	if(unInode->iNodeStat.st_size==0 && numbytes !=0){
		   unInode->Block[0]=allouerUnBlock();
		}
	char DataBlock[BLOCK_SIZE];
	int tailleNouv =min((numbytes + offset), BLOCK_SIZE * N_BLOCK_PER_INODE);
	int octetsAEcrire; //unInode->iNodeStat.st_size-offset;
	char * unFichier = strrchr(pFilename,'/');
		unFichier +=1;

	int n=offset/BLOCK_SIZE;
	int offset_block =offset%BLOCK_SIZE;


	int nombreBlocks = tailleNouv/BLOCK_SIZE;

	 if(tailleNouv%BLOCK_SIZE >0){
		 nombreBlocks +=1;
		} 

if(numbytes >  BLOCK_SIZE * N_BLOCK_PER_INODE){
	printf("Le fichier %s deviendra trop gros!\n",unFichier);
		octetsAEcrire =  BLOCK_SIZE * N_BLOCK_PER_INODE - offset;
		unInode->iNodeStat.st_size =  BLOCK_SIZE * N_BLOCK_PER_INODE;
	}

else if (tailleNouv < unInode->iNodeStat.st_size){
		
		octetsAEcrire=numbytes;
		     
	}else{
		
		octetsAEcrire = tailleNouv - offset;
		unInode->iNodeStat.st_size = tailleNouv;
	
		if(octetsAEcrire >(BLOCK_SIZE-offset)){
		        for(int j=unInode->iNodeStat.st_blocks; j<nombreBlocks; j++){	
			   unInode->Block[j]=allouerUnBlock();
			   unInode->iNodeStat.st_blocks +=1;
			   
			
			} 		    
		}
		
}
       	int x =BLOCK_SIZE-offset_block;
	size_t decalageBlock[N_BLOCK_PER_INODE]={[0 ...N_BLOCK_PER_INODE-1]=0};
	decalageBlock[n]=offset_block;
	int z =0;

      	UINT16 numBlockInode = (unInode->iNodeStat.st_ino/NUM_INODE_PER_BLOCK);
      	WriteBlock(BASE_BLOCK_INODE+numBlockInode,DataBlockInodeEntry);

	for(int j=n; j<nombreBlocks; j++){	
             ReadBlock(unInode->Block[j],DataBlock);
			printf("octet indix %d dans taille debut else %d buffer \n", j,strlen(buffer));
			if(j<nombreBlocks){
				int y=min(256, (octetsAEcrire-(j*256)));
				printf("octet  suivre les  %d de y \n", y);
			if(y<0){break;}
			 x+=y;
			z+=j*y;
			memcpy(&DataBlock[decalageBlock[j]],buffer+z, y);
			printf("octet indix %d dans taille fin else %d buffer %d \n", j,strlen(buffer),z);	

			}		
	printf("octet  ecrit a verif %d de sta \n", x);
		
	WriteBlock(unInode->Block[j],DataBlock);
			
	} 

	return octetsAEcrire;
}


int bd_mkdir(const char *pDirName) {
	ino numInode;
	ino inodeParent;
	ino inodeNew;

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pDirName);
	if(resulInoFichier==0 ){
		 return -2;
		}

	ecrireInodedansBlock(&inodeNew,1);
	if(resulInoFichier !=0){
		return -1;
		}
	
	char *nameOfDir = strrchr(pDirName, '/');
	nameOfDir +=1;
	EcrireRepEntry(&numInode,&inodeNew,nameOfDir,1);
	EcrireRepEntry(&numInode,&inodeNew," . ",1);
	EcrireRepEntry(&numInode,&inodeNew," .. ",1);

	return 0;
}

	
int bd_hardlink(const char *pPathExistant, const char *pPathNouveauLien) {

	ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;

	char DataBlockInodeEntry[BLOCK_SIZE];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pPathExistant);
	if(resulInoFichier !=0 ){
		printf("Le fichier %s est inexistant d\n", pPathExistant);
		return -1;
		}
	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);
	if((unInode->iNodeStat.st_mode & G_IFDIR) && 
	!(unInode->iNodeStat.st_mode & G_IFREG)){
		printf("Le fichier %s n est pas un repertoire \n", pPathExistant);
		return -3;
		}

	ino numInode2 = ROOT_INODE;
	ino inodeParent2 = ROOT_INODE;

	char DataBlockInodeEntry2[BLOCK_SIZE];
	int resulInoFichier2 = getInodeNomFichier(&numInode2,&inodeParent2,pPathNouveauLien);
	if(resulInoFichier2 ==0 ){
		//printf("Le fichier %s est existant d\n", pPathNouveauLien);
		return -2;
		}
	iNodeEntry *unInode2 = chercherInode(&numInode2,DataBlockInodeEntry2);

	char dossierParentPath[256];
	char nomFichier[256];
	GetFilenameFromPath(pPathNouveauLien,nomFichier);
	GetDirFromPath(pPathNouveauLien, dossierParentPath);

	ino numInode3 = ROOT_INODE;
	ino inodeParent3 = unInode->iNodeStat.st_ino;

	char DataBlockInodeEntry3[BLOCK_SIZE];
	int resulInoFichier3 = getInodeNomFichier(&numInode3,&inodeParent3,dossierParentPath);
	if(resulInoFichier3 !=0 ){
		printf("Le repertoire %s est inexistant d\n", dossierParentPath);
		return 1;
		}
	iNodeEntry *unInode3 = chercherInode(&inodeParent3,DataBlockInodeEntry3);


	char nvRepertoireBlock[BLOCK_SIZE];
	printf("Le repertoire %s est inexistant d\n", pPathExistant);
	printf("Le repertoire %s est inexistant d\n", pPathNouveauLien);
	printf("Le repertoire %s est inexistant d\n", dossierParentPath);

	printf("Le inode bloc %d parent est %d \n", unInode2->iNodeStat.st_ino,unInode3->Block[0] );
	ReadBlock(unInode3->Block[0],nvRepertoireBlock);
	printf("Le inode apres lect est %d \n", unInode3->iNodeStat.st_ino);
	DirEntry*linkFile =(DirEntry*) nvRepertoireBlock;
	UINT16 NumEntree = NumberofDirEntry(unInode3->iNodeStat.st_size);
	linkFile[NumEntree].iNode=unInode3->iNodeStat.st_ino;
	//int ofset_linkFile =linkFile->iNode%NUM_INODE_PER_BLOCK;
	printf("Le inode parent est le %d \n", unInode3->iNodeStat.st_ino);
	strcpy(linkFile[NumEntree].Filename, nomFichier);

	unInode->iNodeStat.st_nlink++;
	printf("Le nouvelle du parent est le %d \n", unInode->iNodeStat.st_nlink);
	unInode3->iNodeStat.st_size +=sizeof(DirEntry);

	ajoutInodeDansInode(nomFichier,unInode,unInode3);
	miseAjourInodeDisque(unInode);
	miseAjourInodeDisque(unInode2);
	printf("Le taille ficher copier le %d \n", unInode3->iNodeStat.st_size);


	return 0;
}


int bd_unlink(const char *pFilename) {
	ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;

	char DataBlockInodeEntry[BLOCK_SIZE];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
	printf("Le fichier %s est inexistant d\n", pFilename);
	return -1;
	}
	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);
	if((unInode->iNodeStat.st_mode & G_IFREG)==0){
	return -2;
	}

	unInode->iNodeStat.st_nlink--;

	char dossierParentPath[256];
	char nomFichier[256];
	GetFilenameFromPath(pFilename,nomFichier);
	GetDirFromPath(pFilename, dossierParentPath);

	ino numInode2 = ROOT_INODE;
	ino inodeParent2 = ROOT_INODE;

	char DataBlockInodeEntry2[BLOCK_SIZE];
	int resulInoFichier2 = getInodeNomFichier(&numInode2,&inodeParent2,dossierParentPath);
	if(resulInoFichier2 !=0 ){
	printf("Le repertoire %s est inexistant d\n", dossierParentPath);
	return 1;
	}
	iNodeEntry *unInode2 = chercherInode(&inodeParent2,DataBlockInodeEntry2);
	unInode2->iNodeStat.st_size -=sizeof(DirEntry);

	if(unInode->iNodeStat.st_nlink==0){
	retirerInode(unInode2->iNodeStat.st_ino);
	if(unInode->iNodeStat.st_size!=0){
	ReleaseFreeBlock(unInode2->Block[0]);
	}
	}

	char nvRepertoireBlock[BLOCK_SIZE];
	ajoutInodeDansInode(nomFichier,unInode,unInode2);
	miseAjourInodeDisque(unInode);
	miseAjourInodeDisque(unInode2);
	
	return 0;
}

int bd_rmdir(const char *pFilename) {
	return -1;
}


int bd_rename(const char *pFilename, const char *pDestFilename) {
	char sourceDirectory[256];
	char sourceFileName[FILENAME_SIZE];
	char DestinationDirectory[256];
	char DestinationFileName[FILENAME_SIZE];
	ino numInodeFileSource = ROOT_INODE;
	ino numInodeFileSourceParent = ROOT_INODE;
	ino numInodeFileDestination = ROOT_INODE;
	ino numInodeFileDestinationParent = ROOT_INODE;
	char DataBlockInodeSource[BLOCK_SIZE];
	char DataBlockInodeSourceParent[BLOCK_SIZE];
	char DataBlockInodeDestination[BLOCK_SIZE];
	char DataBlockInodeDestinationParent[BLOCK_SIZE];
	// on teste si la source et la destination sont identiques, on ne fait rien
	if (strcmp(pFilename, pDestFilename) == 0){
    		return 0;
		}
	// on teste si le fichier source existe ou pas
	if (GetFilenameFromPath(pFilename, sourceFileName) == 0){
	    	return -1;
		}
	// on teste si le chemin d'acces au ficheir existe
	if (GetDirFromPath(pFilename, sourceDirectory) == 0){
		return -1;
		}
	if (getInodeNomFichier(&numInodeFileSource, &numInodeFileSourceParent, pFilename) == -1){
    		return -1;	
		}
	iNodeEntry *pInodeFileSourceParent = chercherInode(&numInodeFileSourceParent, DataBlockInodeSourceParent);
	iNodeEntry *pInodeFileSource = chercherInode(&numInodeFileSource, DataBlockInodeSource);

	if (GetDirFromPath(pDestFilename, DestinationDirectory) == 0){
		return -1;
		}
	if (GetFilenameFromPath(pDestFilename, DestinationFileName) == 0){
		return -1;
		}
	if (getInodeNomFichier(&numInodeFileDestination, &numInodeFileDestinationParent, pFilename) == -1){
		return -1;
		}
	iNodeEntry *pInodeFileDestination = chercherInode(&numInodeFileDestination,DataBlockInodeDestination);	
	iNodeEntry *pInodeFileDestinationParent = chercherInode(&numInodeFileDestinationParent,DataBlockInodeDestinationParent);

	if (pInodeFileSource->iNodeStat.st_mode & G_IFDIR) {
		if (strcmp(sourceDirectory, DestinationDirectory) != 0) {
			pInodeFileSourceParent->iNodeStat.st_nlink--;
      			pInodeFileDestinationParent->iNodeStat.st_nlink++;
    		}
    		
		char dataBlock[BLOCK_SIZE];
    		if (ReadBlock(pInodeFileSource->Block[0], dataBlock) == -1){
      			return -1;
			}
    		DirEntry *pDirEntry = (DirEntry*)dataBlock;
   	 	pDirEntry[1].iNode = pInodeFileDestinationParent->iNodeStat.st_ino;

    		if (WriteBlock(pInodeFileSource->Block[0], dataBlock) == -1){
      			return -1;
  			}
	}


  	return 0;

}

int bd_readdir(const char *pDirLocation, DirEntry **ppListeFichiers) {
	ino numInode;
	ino inodeParent;

	
	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pDirLocation);
	if(resulInoFichier !=0 ){
	return -1;
	}

	char DataBlockInodeEntry[BLOCK_SIZE];
	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);

	if(!unInode->iNodeStat.st_mode & G_IFDIR){
	printf("L inode %d n est pas un repertoire\n", numInode);
	return -1;
	}
        printf("L inode %d  est enfant\n", numInode);
	char DataBlockDirEntry[BLOCK_SIZE];
	ReadBlock(unInode->Block[0],DataBlockDirEntry);
	DirEntry *phDirEntry = (DirEntry*)DataBlockDirEntry;
        (*ppListeFichiers) = (DirEntry*) malloc(unInode->iNodeStat.st_size);
	memcpy(*ppListeFichiers,phDirEntry,unInode->iNodeStat.st_size);		

	return unInode->iNodeStat.st_size/sizeof(DirEntry);
}


int bd_truncate(const char *pFilename, int NewSize) {

	ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;
	//ino inodeNouv;

	char DataBlockInodeEntry[BLOCK_SIZE];

	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
		printf("Le fichier %s est inexistant \n", pFilename);
	 	return -1;
		}

	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);
	if(unInode->iNodeStat.st_mode & G_IFDIR){
		printf("Le fichier %s est un repertoire \n", pFilename);
		return -2;
		}

	if(NewSize > unInode->iNodeStat.st_size){
		return unInode->iNodeStat.st_size;
		}

	if(NewSize==0){
		ReleaseFreeBlock(unInode->Block[0]);
		printf("On doit libérer un bloc de données pour la prochaine opération \n");
		}

	unInode->iNodeStat.st_size = NewSize;

	UINT16 numBlockInode = (unInode->iNodeStat.st_ino/NUM_INODE_PER_BLOCK);
	
	WriteBlock(BASE_BLOCK_INODE+numBlockInode,DataBlockInodeEntry);
	
	return NewSize;
}



int bd_formatdisk() {
	return -1;
}



int bd_chmod(const char *pFilename, UINT16 st_mode) {
        ino numInode = ROOT_INODE;
	ino inodeParent = ROOT_INODE;
	//const char *temp;
	
	int resulInoFichier = getInodeNomFichier(&numInode,&inodeParent,pFilename);
	if(resulInoFichier !=0 ){
		return -1;
		}

	
	char DataBlockInodeEntry[BLOCK_SIZE];
	iNodeEntry *unInode = chercherInode(&numInode,DataBlockInodeEntry);

	UINT16 numBlockInode = (unInode->iNodeStat.st_ino/NUM_INODE_PER_BLOCK);

	unInode->iNodeStat.st_mode = st_mode | (unInode->iNodeStat.st_mode & (G_IFREG | G_IFDIR));
        //printf("%d numero inode avant modification dans chod\n", unInode->iNodeStat.st_ino);
	WriteBlock(BASE_BLOCK_INODE+numBlockInode,DataBlockInodeEntry);
	

	//printf("%d numero block inode chod\n",numBlockInode);
	//int nbreRepOccuper = unInode->iNodeStat.st_size/sizeof(phDirEntry);
	//printf("%d numero inode avant modification dans chod\n",numInode);
	//unInode->iNodeStat.st_mode |=G_IRWXU |G_IRWXG|G_IFREG;

	return 0;
}

int bd_fct_perso(){ //ajuster aussi les paramètres
	return -1;
}
