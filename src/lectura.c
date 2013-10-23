#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _persona {
    struct _persona * Madre;
    struct _persona * Padre;
    char nombre [20];
    struct _persona *sig;
} Persona;

typedef struct _familia {
    char  nomb_Familia [20];
    char  apellido1 [20];
    char  apellido2 [20];
    Persona *Madre;
    Persona *Padre ;
    Persona *Hijos [5];
    struct _familia *sig;
} Familia;


void buffer(void);
Persona *Buscar(char *, Persona *);
Persona * Crear(Persona *,char *,char *,char *);
void Reemplazar(Persona *,char *,char *,char *);
void Imprimir(Persona *);
void Imprimir_fam(Familia *);
void Recortar(char[50],char[50],int,int);
Familia * Crear_familia(Persona *,char *,char *,char *);

int main() {
	//~ Se define un archivo para trabajarlo
    FILE *archivo; 
    //~ Creamos dos variables de Persona Ref tiene la estructura inicial del problema y Pers indica el elemento actual
    Persona *pers,*ref=(Persona*)malloc (sizeof(Persona));
	Familia *ref_fam=(Familia*)malloc(sizeof(Familia)),*fami;
    int empieza=0,empieza_fam=0;
    char caracteres[50],aux[50],papa[50]="",hijo[50]="",mama[50]="",apellido[50],famil_nomb[50];
    //~ creo las variables de nombre que ayudaran en el proceso
    archivo = fopen("Test.txt","r");
    //~ reviso si el arhivo fue cargado o salgo del programa
    if (archivo == NULL) exit(1);
    //~ en caso de entrar al programa empieza a leer el archivo
    while (feof(archivo) ==0) {
        fgets(caracteres,50,archivo);
        //~ Revisa si la linea corresponde al padre
        if(strncmp(caracteres,"Nombre de la familia",10)==0){
			fgets(caracteres,50,archivo);
			strcpy(famil_nomb,caracteres);
		}
        else if (strncmp(caracteres,"Padre: ",7)==0) {
			//~ Corta la cadena solo con el nombre
			Recortar(caracteres,papa,7,strlen(caracteres));
			//~ Imprime el nombre para corroborar
            //~ Busca en la lista si el nombre ya fue ingresado
            if(Buscar(papa,ref)!=NULL) {
                printf("\nFue ingresado\n");
                
            }
            else if(empieza==0){
				ref=Crear(NULL,papa,"","");
				printf("  Creando.....\n");
				//~ Le indico a pers la posicion actual de ref para empezar a trabajar con el
				pers=ref;
				empieza++;
				
			}
            else{
				pers->sig=Crear(ref,papa,"","");
				pers=pers->sig;
				printf("  Creando.....\n");
			}
            int i=0;
            while(papa[i]!=' ') i++;
            i++;
            Recortar(papa,apellido,i,strlen(papa));
        }
        else if (strncmp(caracteres,"Madre: ",7)==0) {
            Recortar(caracteres,mama,7,strlen(caracteres));
            printf("%s",mama);
            if(Buscar(mama,ref)!=NULL) {
                printf("  Fue ingresado\n");
            }
            else{
				pers->sig=Crear(ref,mama,"","");
				pers=pers->sig;
				printf("  Creando.....\n");
			}
			if(empieza_fam==0){
				ref_fam=Crear_familia(ref,famil_nomb,papa,mama);
				fami=ref_fam;
				empieza_fam++;
			}
			else{
				fami->sig=Crear_familia(ref,famil_nomb,papa,mama);
				fami=fami->sig;
			}
        }
        else if (strncmp(caracteres,"Hijos: ",7)==0) {
            Recortar(caracteres,hijo,7,strlen(caracteres));
            if(strncmp(caracteres,"Hijos: sin hijos",16)==0) { 
				int x=0;
				for(x=0;x<50;x++){
					apellido[x]='\0';
					papa[x]='\0';
					caracteres[x]='\0';
					aux[x]='\0';
					hijo[x]='\0';
					mama[x]='\0';
					famil_nomb[x]='\0';
				}
				
			} 
            else {
				int len=0,num=0,verificador=0,num_hijo=0;
				while(verificador==0){
					num=0;
					while(hijo[len]!=',' && hijo[len]!='\n'&& hijo[len]!='\r'&& hijo[len]!=' ' ){
						aux[num]=hijo[len];
						len++;
						num++;
					}
					aux[num]=' ';
					aux[num+1]='\0';
					len+=2;
					printf("XXXXXXXXXXXXXXXXXXXXXXXXx %s%s",aux,apellido);
					if(aux[strlen(aux)-1]!=' '){
						strcat(aux," ");
					}
					strcat(aux,apellido);
					if(Buscar(aux,ref)!=NULL) {
						printf("  Fue ingresado\n");
						Reemplazar(ref,aux,papa,mama);
						fami->Hijos[num_hijo]=Buscar(aux,ref);
						num_hijo++;
					}			
					else{
						pers->sig=Crear(ref,aux,papa,mama);
						pers=pers->sig;
						printf(" Creando.....\n");
						fami->Hijos[num_hijo]=Buscar(aux,ref);
						num_hijo++;
						
					}
					if(hijo[len-2]=='\n'|| hijo[len-2]=='\r'|| hijo[len-2]==' ' ){
						verificador=1;
						int x=0;
						for(x=0;x<50;x++){
							apellido[x]='\0';
							papa[x]='\0';
							caracteres[x]='\0';
							aux[x]='\0';
							hijo[x]='\0';
							mama[x]='\0';
							famil_nomb[x]='\0';
						}
					}
					
				}
				
				
			}            
        }
        else{
			 printf("%s",caracteres);
		}
	
    }
    Imprimir(ref);
    Imprimir_fam(ref_fam);
    fclose(archivo);
    return 0;
}

void Imprimir(Persona *ref){
FILE *verific=fopen("verificador_personas.txt","w");
    Persona *a=ref;
	while(a!=NULL){
		fputs( "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n", verific );
		printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
		puts(a->nombre);
		fputs("\nNombre: ", verific );
		fputs(a->nombre, verific );
		if(a->Madre==NULL){
			puts(" ");
			fputs("\nMadre: ", verific );
			fputs(" ", verific );
		}
		else{
			puts(a->Madre->nombre);
			fputs("\nMadre: ", verific );
			fputs(a->Madre->nombre, verific );
		}
		if(a->Padre==NULL){
			puts(" ");
			fputs("\nPadre: ", verific );
			fputs(" ", verific );
		}
		else{
		puts(a->Padre->nombre);
		fputs("\nPadre: ", verific );
		fputs(a->Padre->nombre, verific );
		}
		a=a->sig;
	}
	fclose (verific);	
}

void Imprimir_fam(Familia *ref){
FILE *verific=fopen("verificador_familias.txt","w");
    Familia *a=ref;
	while(a!=NULL){
		fputs( "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n", verific );
		puts("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

		fputs("\nNombre: ", verific );
		puts(a->nomb_Familia);
		fputs(a->nomb_Familia, verific );
		if(a->Padre==NULL){
			puts(" ");
			fputs("\nPadre: ", verific );
			fputs(" ", verific );
		}
		else{
			fputs("\nPadre: ", verific );
			puts(a->Padre->nombre);
			fputs(a->Padre->nombre, verific );
		}
		if(a->Madre==NULL){
			puts(" ");
			fputs("\nMadre: ", verific );
			fputs(" ", verific );
		}
		else{
			fputs("\nMadre: ", verific );
			puts(a->Madre->nombre);
			fputs(a->Madre->nombre, verific );
		}
		int i=0;
		while(a->Hijos[i]!=NULL && i<5){
			fputs("\nHijo: ", verific );
			puts(a->Hijos[i]->nombre);
			fputs(a->Hijos[i]->nombre, verific );
			i++;
		}
		a=a->sig;
	}
	fclose (verific);	
}

Persona * Crear(Persona *ref,char *name,char *Pap,char *Mom){
	Persona *pers=(Persona*)malloc (sizeof(Persona));
	strcpy(pers->nombre,name);
	pers->Madre=Buscar(Mom,ref);
	pers->Padre=Buscar(Pap,ref);
	pers->sig=NULL;
	return pers;
}

Familia * Crear_familia(Persona *ref,char *name,char *Pap,char *Mom){
	Familia *pers=(Familia*)malloc (sizeof(Familia));
	strcpy(pers->nomb_Familia,name);
	pers->Madre=Buscar(Mom,ref);
	pers->Padre=Buscar(Pap,ref);
	pers->sig=NULL;
	return pers;
}

void Recortar(char string[],char substr[],int base,int len){
	int i=0;
	printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
	while(base<len && string[base]!='\n' && string[base]!='\0'){
		printf("%c",string[base]);
		substr[i]=string[base];
		base++;
		i++;
	}
	substr[base+i]='\0';
	printf("\n--------%s",string);
	printf("\n........%s\n",substr);
	printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
}

void Reemplazar(Persona *pers,char *name,char *Pap,char *Mom){
	Persona *a=pers;
	while(a!=NULL){
		
		if(strncmp(a->nombre,name,10)==0 && a->Padre==NULL){
			pers->Madre=Buscar(Mom,pers);
			pers->Padre=Buscar(Pap,pers);
		}
		a=a->sig;
	}
}

Persona* Buscar(char *personaje, Persona *prim) {
    if(prim==NULL) {
        return NULL;
    }
    else {
        Persona *a=NULL;
        a=prim;
        
        while(a->sig!=NULL) {
            if(strncmp(a->nombre,personaje,8)==0) {
                return a;
            }
            else {
                a=a->sig;
            }
        }
        if(strncmp(a->nombre,personaje,8)==0) {
            return a;
        }
        else {
            return NULL;
        }
    }
}

void buffer(void) {
    while(getchar()!='\n') {}
}
