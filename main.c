#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define SIZEAir 500
#define SIZEfly 500

#pragma region  structs

//Estrutura para guardar informação util ao funcionamento de certas funções
typedef struct
{
    char str[50];
}Read;

//Estrutura para a lista de origens de cada aeroporto
typedef struct _Origins
{
    char code[10];
    struct _Origins* next;
}ListOrigins;

//Estrutura para a lista de destinos de cada aeroporto
typedef struct _Destinations
{
    char code[10];
    float price;
    struct _Destinations* next;
}ListDestinations;

//Estrutura para a lista do aeroportos
typedef struct _Airports 
{
    char city[50];
    char country[50];
    char code[10];
    Read read[18000];
    ListDestinations* listDestinations;
    ListOrigins* listOrigins;
    struct _Airports *next;
}ListAirports;

//Estrutura para a lista dos voos
typedef struct _Flights
{
    char origin[10];
    char destination[10];
    float distance;
    float cost;
    float duration;
    char type[50];
    struct _Flights* next;
}ListFlights;

//Estrutura para a Hash Table do voos
typedef struct _HashFlights
{
    ListFlights**list;
    int size;
}HashFlights;

//Estrutura para a Hash Table do aeroportos
typedef struct _HashAirports
{
    ListAirports**list;
    int size;
}HashAirports;

#pragma endregion

#pragma region  funções Hash

//Função para devolver posição na hash
int hash_it(const char* x, int size) 
{
    int tot = 0, len = strlen(x);

    for (int i = 0; i < len; i++) 
    {
        tot += x[i];
    }
    return tot % size;
}

//Função para devolver posição na hash
unsigned int Hash(char *key, unsigned int tableSize) 
{
    unsigned int hashVal = 0;
    while( *key != '\0' )
        hashVal += *key++;
    return hashVal % tableSize;
}

#pragma endregion

#pragma region airports

//Função para criar a Hash Table dos aeroportos
HashAirports* create_hash_airports(int size)
{
    HashAirports* hash=(HashAirports*) malloc(sizeof(hash));
    hash->size= size;
    hash->list=(ListAirports**)malloc(size*sizeof(ListAirports*));

    
    for (int i = 0; i < hash->size; i++)
    {
        hash->list[i]=NULL;
    }
    return hash;
}

//Função para inserir aeroportos numa lista
ListAirports* insert_airports(ListAirports* lst, char city[50], char country[50], char code[10])
{
    ListAirports*aux,*new_cell;
    new_cell = ( ListAirports*) malloc (sizeof(ListAirports));
    strcpy(new_cell->city, city);
    strcpy(new_cell->country, country);
    strcpy(new_cell->code, code);
    new_cell->listDestinations=(ListDestinations*)malloc(SIZEAir*sizeof(ListDestinations*));
    new_cell->next = NULL;

    if(lst) 
    {
        for(aux = lst ; aux->next ; aux = aux->next) 
        { }
        aux->next = new_cell;
    }
    else
    {
        lst = new_cell;
    }
    return lst;
}

//Função para inserir aeroportos na Hash Table
void insert_hash_airports(HashAirports* hash, char city[50], char country[50], char code[10])
{
    int h = hash_it(code, hash->size);
    hash->list[h]=insert_airports(hash->list[h], city, country, code);
}

//Função para listar os aeroportos presentes numa lista
void airports_visit(ListAirports* aux)
{
    while (aux!=NULL)
    {
        printf("%s|%s|%s___________", aux->city, aux->country, aux->code);
        aux=aux->next;
    }
}

//Função para listar os aeroportos presentes na Hash Table
void hashAirports_visit(HashAirports* hash)
{
    for (int i = 0; i < hash->size; i++)
    {
        printf("%d==>", i);
        airports_visit(hash->list[i]);
        printf("\n");
    }
}
#pragma endregion

#pragma region Flights

//Função para criar a Hash Table dos voos
HashFlights* create_hash_flights(int size)
{
    HashFlights* hash=(HashFlights*) malloc(sizeof(hash));
    hash->size= size;
    hash->list=(ListFlights**)malloc(size*sizeof(ListFlights*));
    for (int i = 0; i < hash->size; i++)
    {
        hash->list[i]=NULL;
    }
    return hash;
}

//Função para inserir origens na lista de origens de um aeroporto
ListOrigins* insert_origins(ListOrigins* lst, char code[10])
{
    ListOrigins* new_cell, *aux;
    new_cell = (ListOrigins*) malloc(sizeof(ListOrigins));
    strcpy(new_cell->code, code);
    new_cell->next=NULL;
    if(lst) 
    {
        for(aux = lst ; aux->next ; aux = aux->next) 
        { }
        aux->next = new_cell;
    }
    else
    {
        lst = new_cell;
    }
    return lst;
}

//Função para inserir destinos na lista de destinos de um aeroporto
ListDestinations* insert_destinations(ListDestinations* lst, char code[10], float price)
{
    ListDestinations* new_cell, *aux;
    new_cell = (ListDestinations*) malloc(sizeof(ListDestinations));
    strcpy(new_cell->code, code);
    new_cell->price=price;
    new_cell->next=NULL;
    if(lst) 
    {
        for(aux = lst ; aux->next ; aux = aux->next) 
        { }
        aux->next = new_cell;
    }
    else
    {
        lst = new_cell;
    }
    return lst;
}

//Função para calcular preço dos voos
float calc_price(char type[10], float duration, float distance)
{
    float cost;
    if (strcasecmp(type, "lowcost")==0)
    {
        cost=0.5*duration+0.45*distance + 0.05*0;
    }
    else
    {
        cost=0.5*duration+0.45*distance + 0.05*1;
    }
    return cost;
}

//Função para inserir voos numa lista
ListFlights* insert_flights(ListFlights* lst, char origin[10] ,float distance, float duration, char type[50], char destination[10])
{
    ListFlights*aux,*new_cell;
    new_cell = ( ListFlights*) malloc (sizeof(ListFlights));
    float cost;
    strcpy(new_cell->origin, origin);
    strcpy(new_cell->type, type);
    strcpy(new_cell->destination, destination);
    new_cell->distance=distance;
    new_cell->duration=duration;
    cost=calc_price(type, duration, distance);
    new_cell->cost=cost;
    new_cell->next = NULL;

    if(lst) 
    {
        for(aux = lst ; aux->next ; aux = aux->next) 
        { }
        aux->next = new_cell;
    }
    else
    {
        lst = new_cell;
    }
    return lst;
    
}

//Função para inserir voos na Hash Table
void insert_hash_flights(HashFlights* hash, HashAirports* hashAirports, char origin[10], float distance, float duration, char type[50], char destination[10])
{   
    int h = Hash(origin, hash->size);
    
    hash->list[h]=insert_flights(hash->list[h], origin, distance, duration, type, destination);
    
    int pos = Hash(origin, hashAirports->size);
    
    int cost;
    if (strcasecmp(hashAirports->list[pos]->code, origin)==0);
    {
        cost = calc_price(type, duration, distance);
        hashAirports->list[pos]->listDestinations=insert_destinations(hashAirports->list[pos]->listDestinations, destination, cost);
    }

    pos = Hash(destination, hashAirports->size);
    if (strcasecmp(hashAirports->list[pos]->code, destination)==0);
    {
        hashAirports->list[pos]->listOrigins=insert_origins(hashAirports->list[pos]->listOrigins, origin);
    }
}

//Função para listar os voos de uma lista
void flights_visit(ListFlights* aux)
{
    while (aux!=NULL)
    {
        printf("%s|%f|%f|%s|%s/%f//////////////", aux->origin,aux->distance, aux->duration, aux->type, aux->destination, aux->cost);
        aux=aux->next;
    }
}

//Função para listar os voos da Hash Table
void hashFlights_visit(HashFlights* hash)
{
    for (int i = 0; i < hash->size; i++)
    {
        printf("%d==>", i);
        flights_visit(hash->list[i]);
        printf("\n");
    }
}

#pragma endregion

#pragma region Ficheiros

//Função para ler o ficheiro dos Aeroportos
void read_file_airports(HashAirports* hash)
{
    FILE* airports = fopen("airports-PT.tsv", "rt");
    int i=0;
    char str[200], fLine[100], city[50], country[50], code[10], trash[100];

    if (airports==NULL)
    {
        perror("Unable to open the file requested!");
    }
    else
    {
        while (!feof(airports))
        {
            //Se o i for 0 entao não vai ler nada
            if (i==0)
            {
                fgets(fLine, sizeof(fLine), airports);  
            }
            //Se i não for 0 então vai ler os dados normalmente
            else
            {
                fscanf(airports, "%[^\t]\t%[^\t]\t%s\n", city, country, code);
                insert_hash_airports(hash, city, country, code);
            }
            i++;
        }
    }
    fclose(airports);
}

//Função para ler o ficheiro dos Voos
void read_file_flights(HashFlights* hashFlights, HashAirports* hashAirports)
{
    FILE* flights = fopen("flights-PT.tsv", "rt");
    int i=0;
    char origin[10], type[50], destination[10], str[200], fLine[100];
    float distance, duration;

    if (flights==NULL)
    {
        perror("Unable to open the file requested!");
    }
    else
    {
        while (!feof(flights))
        {
            //Se o i for 0 entao não vai ler nada
            if (i==0)
            {
                fgets(fLine, sizeof(fLine), flights);  
                
            }
            //Se i não for 0 então vai ler os dados normalmente
            else
            {
                fscanf(flights, "%[^\t]\t%f\t%f\t%[^\t]\t%s\n", origin, &distance, &duration, type, destination);
                insert_hash_flights(hashFlights, hashAirports, origin, distance, duration, type, destination);
            }
            i++;
        }
    }
    fclose(flights);
}

#pragma endregion

#pragma region Metodos e procedimentos

//1ª-Procedimento que determina todas as cidades de destino a partir de uma certa origem
void possible_destinations(HashFlights* hashFlights, HashAirports* hashAirports)
{
    char input[10];
    int layOver, pos, h, count=0, count2=0, count3=0;
    ListDestinations*aux, *aux2, *aux3;
    ListAirports *aux4, *aux5, *codeAux;
    bool visited=false;
    Read read[10000], read2[10000], read3[10000];
    ListFlights* lst;
    printf("City of origin: ");
    scanf("%[^\n]s", input);
    printf("Number of lay-overs(0,1 or 2): ");
    scanf("%d", &layOver);

    switch (layOver)
    {
        case 0:
        for (int i = 0; i < hashAirports->size; i++)
        {
            aux4 = hashAirports->list[i];
            while (aux4)
            {
                pos = Hash(aux4->code, hashAirports->size);
                codeAux=hashAirports->list[pos];
                if (strcasecmp(codeAux->city, input)==0)
                {
                    aux=aux4->listDestinations;
                    while (aux)
                    {
                        visited=false;
                        for (int i = 0; i < count; i++)
                        {
                            if (strcasecmp(aux->code, read[i].str)==0)
                            {
                                visited=true;
                            }
                        }
                        if (!visited)
                        {
                            printf("%s\n", aux->code);
                            strcpy(read[count].str, aux->code);
                            count++;
                        }
                        aux=aux->next;
                    }
                }
                aux4=aux4->next;
            }
        }
        printf("%d\n", count);
        break;
    case 1:
        for (int i = 0; i < hashAirports->size; i++)
        {
            aux4 = hashAirports->list[i];
            while (aux4)
            {
                pos = Hash(aux4->code, hashAirports->size);
                codeAux=hashAirports->list[pos];
                if (strcasecmp(codeAux->city, input)==0)
                {
                    aux=aux4->listDestinations;
                    while (aux)
                    {
                        visited=false;
                        for (int i = 0; i < count; i++)
                        {
                            if (strcasecmp(aux->code, read[i].str)==0)
                            {
                                visited=true;
                            }
                        }
                        if (!visited)
                        {
                            printf("%s_0 lay-overs\n", aux->code);
                            strcpy(read[count].str, aux->code);
                            count++;
                        }
                        pos=Hash(aux->code, hashAirports->size);
                        aux5=hashAirports->list[pos];
                        while (aux5)
                        {
                            aux2=aux5->listDestinations;
                            while (aux2)
                            {
                                visited=false;
                                for (int i = 0; i < count2; i++)
                                {
                                    if (strcasecmp(aux2->code, read2[i].str)==0)
                                    {
                                        visited=true;
                                    }
                                }
                                if (!visited)
                                {
                                    printf("%s_1 lay-over \n", aux2->code);
                                    strcpy(read2[count2].str, aux2->code);
                                    count2++;
                                }
                                aux2=aux2->next;
                            }
                            aux5=aux5->next;
                        }
                        aux=aux->next;
                    }
                }
                aux4=aux4->next;
            }
        }
        printf("%d\n", count+count2);
        break;
    case 2:
        for (int i = 0; i < hashAirports->size; i++)
        {
            aux4 = hashAirports->list[i];
            while (aux4)
            {
                pos = Hash(aux4->code, hashAirports->size);
                codeAux=hashAirports->list[pos];
                if (strcasecmp(codeAux->city, input))
                {
                    aux=aux4->listDestinations;
                    while (aux)
                    {
                        visited=false;
                        for (int i = 0; i < count; i++)
                        {
                            if (strcasecmp(aux->code, read[i].str)==0)
                            {
                                visited=true;
                            }
                        }
                        if (!visited)
                        {
                            printf("%s_0 lay_overs\n", aux->code);
                            strcpy(read[count].str, aux->code);
                            count++;
                        }
                        pos=Hash(aux->code, hashAirports->size);
                        aux2=hashAirports->list[pos]->listDestinations;
                        while (aux2)
                        {
                            visited=false;
                            for (int i = 0; i < count2; i++)
                            {
                                if (strcasecmp(aux2->code, read2[i].str)==0)
                                {
                                    visited=true;
                                }
                            }
                            if (!visited)
                            {
                                printf("%s_1 lay_overs\n", aux2->code);
                                strcpy(read2[count2].str, aux2->code);
                                count2++;
                            }
                            pos= Hash(aux2->code, hashAirports->size);
                            aux3=hashAirports->list[pos]->listDestinations;
                            while (aux3)
                            {
                                visited=false;
                                for (int i = 0; i < count3; i++)
                                {
                                    if (strcasecmp(aux3->code, read3[i].str)==0)
                                    {
                                        visited=true;
                                    }
                                }
                                if (!visited)
                                {
                                    printf("%s_2 lay_overs\n", aux3->code);
                                    strcpy(read3[count3].str, aux3->code);
                                    count3++;
                                }
                                aux3=aux3->next;
                            }
                            aux2=aux2->next;
                        }
                        aux=aux->next;
                    }
                }
                aux4=aux4->next;
            }
        }
        printf("%d\n", count + count2 + count3);
        break;
    default:
        break;
    }
}

//2ª-A cidade com mais partidas diferentes para cidades distintas 
void city_most_different_flights_from(HashFlights* hashFlights, HashAirports* hashAirports)
{
    ListFlights* auxFli;
    ListAirports* auxAir;
    ListDestinations* auxDest;
    Read read[19000];
    Read cities[10000];
    bool visited=false, saved=false;
    int count=0, readElements=0, winnerCount=0, readCities=0;
    Read winner;
    
    for (int i = 0; i < hashAirports->size; i++)
    {
        auxAir=hashAirports->list[i];
        while (auxAir)
        {
            saved=false;
            for (int i = 0; i < readCities; i++)
            {
                if (strcasecmp(auxAir->city, cities[i].str)==0)
                {
                    saved=true;
                }
            }
            if (!saved)
            {
                strcpy(cities[readCities].str, auxAir->city);
                readCities++;
            }
            auxAir=auxAir->next;
        }
    }
    for (int i = 0; i < readCities; i++)
    {
        for (int j = 0; j < hashAirports->size; j++)
        {
            readElements=0;
            auxAir=hashAirports->list[j];
            while (auxAir)
            {
                if (strcasecmp(auxAir->city, cities[i].str)==0)
                {
                    auxDest=auxAir->listDestinations;
                    while (auxDest)
                    {
                        if (strcasecmp(auxDest->code, auxAir->code)!=0)
                        {
                            visited=false;
                            for (int k = 0; k < readElements; k++)
                            {
                                if (strcasecmp(auxDest->code, read[k].str)==0)
                                {
                                    visited=true;
                                }
                            }
                            if (!visited)
                            {
                                strcpy(read[readElements].str, auxDest->code);
                                readElements++;
                            }
                        }            
                        auxDest=auxDest->next;
                    }
                    if (readElements>winnerCount)
                    {
                        strcpy(winner.str,cities[i].str);
                        winnerCount=readElements;
                    }
                }
                auxAir=auxAir->next;
            }
        }
    }
    printf("%s %d\n", winner.str, winnerCount);

}

//3ª-A cidade com mais voos diferentes vindos de cidades distintas 
void city_most_different_flights_to(HashFlights* hashFlights, HashAirports* hashAirports)
{
    ListFlights* auxFli;
    ListAirports* auxAir;
    ListOrigins* auxOrig;
    Read read[19000];
    Read cities[10000];
    bool visited=false, saved=false;
    int count=0, readElements=0, winnerCount=0, readCities=0;
    Read winner;
    
    for (int i = 0; i < hashAirports->size; i++)
    {
        auxAir=hashAirports->list[i];
        while (auxAir)
        {
            saved=false;
            for (int i = 0; i < readCities; i++)
            {
                if (strcasecmp(auxAir->city, cities[i].str)==0)
                {
                    
                    saved=true;
                }
            }
            if (!saved)
            {
                strcpy(cities[readCities].str, auxAir->city);
                readCities++;
            }
            auxAir=auxAir->next;
        }
    }
    for (int i = 0; i < readCities; i++)
    {
        for (int j = 0; j < hashAirports->size; j++)
        {
            auxAir=hashAirports->list[j];
            while (auxAir)
            {
                readElements=0;
                if (strcasecmp(auxAir->city, cities[i].str)==0)
                {
                    auxOrig=auxAir->listOrigins;
                    while (auxOrig)
                    {
                        if (strcasecmp(auxOrig->code, auxAir->code)!=0)
                        {
                            visited=false;
                            for (int k = 0; k < readElements; k++)
                            {
                                if (strcasecmp(auxOrig->code, read[k].str)==0)
                                {
                                    visited=true;
                                }
                            }
                            if (!visited)
                            {
                                strcpy(read[readElements].str, auxOrig->code);
                                readElements++;
                            }
                        }            
                        auxOrig=auxOrig->next;
                    }
                    if (readElements>winnerCount)
                    {
                        strcpy(winner.str,cities[i].str);
                        winnerCount=readElements;
                    }
                }
                auxAir=auxAir->next;
            }
        }
    }
    printf("%s %d\n", winner.str, winnerCount);
}

//4º-Procedimento para devolver o aeroporto mais internacional de um dado país
void most_international_airport(HashAirports* hashAirports)
{
    ListAirports* auxAirports, *auxAirports2, *winner;
    ListDestinations* auxDestinations;
    char country[50], copied[50];
    int count=0, winnerCount=0, pos;

    printf("Type the name of the country: ");
    scanf("%[^\n]", country);

    for (int i = 0; i < hashAirports->size; i++)
    {
        auxAirports=hashAirports->list[i];
        while (auxAirports)
        {
            count=0;
            if (strcasecmp(auxAirports->country, country)==0)
            {
                auxDestinations = auxAirports->listDestinations;
                while (auxDestinations)
                {
                    pos = Hash(auxDestinations->code, hashAirports->size);
                    auxAirports2 = hashAirports->list[pos];
                    while (auxAirports2)
                    {
                        if (strcasecmp(auxAirports2->code, auxDestinations->code)==0)
                        {
                            strcpy(copied,auxAirports2->country);
                        }
                        auxAirports2=auxAirports2->next;
                    }
                    if (strcasecmp(copied, auxAirports->country)!=0)
                    {
                        count++;
                    }
                    auxDestinations=auxDestinations->next;
                }
            }        
            if (count>winnerCount)
            {
                winnerCount=count;
                winner=auxAirports;
            }
            auxAirports=auxAirports->next;
        }    
    }
    printf("%s\n", winner->code);
    
}

//5ª-Procedimento para determinar a melhor opção de voo entre dois aeroportos
void best_flight_option_airports(HashFlights* hashFlights, HashAirports*hashAirports)
{
    char origin[10], destination[10], flight1[10], flight2[10], flight3[10], winner1[10], winner2[10], winner3[10];
    float sum=0, sum1=0, sum2=0, smallest=200000;
    int pos, count=0, bcount=0;
    ListAirports* auxAir, *auxAir2, *auxAir3;
    ListDestinations* auxDest, *auxDest2, *auxDest3;
    bool possible=false;

    fflush(stdin);
    printf("Airport of departure: ");
    scanf("%[^\n]s", origin);
    fflush(stdin);
    printf("Airport of arrival: ");
    scanf("%[^\n]s", destination);

    pos = Hash(origin, hashAirports->size);
    auxAir=hashAirports->list[pos];
    while (auxAir)
    {
        if (strcasecmp(auxAir->code, origin)==0)
        {
            auxDest=auxAir->listDestinations;
            possible=false;
            sum=0;
            while (auxDest)
            {
                if (strcasecmp(auxDest->code, destination)==0)
                {
                    sum+=auxDest->price;
                    count++;
                    strcpy(flight1,auxDest->code);
                    strcpy(flight2, "0");
                    strcpy(flight3, "0");
                    possible=true;
                }
                else
                {
                    pos = Hash(auxDest->code, hashAirports->size);
                    auxAir2 = hashAirports->list[pos];
                    sum+=auxDest->price;
                    strcpy(flight1, auxDest->code);
                    strcpy(flight2, "0");
                    strcpy(flight3, "0");
                    count++;
                    while (auxAir2)
                    {
                        if (strcasecmp(auxAir2->code, auxDest->code)==0)
                        {
                            auxDest2=auxAir2->listDestinations;
                            while (auxDest2)
                            {
                                if (strcasecmp(auxDest2->code, destination)==0)
                                {
                                    sum+=auxDest2->price;
                                    possible=true;
                                    count++;
                                    strcpy(flight2, auxDest2->code);
                                    strcpy(flight3, "0");
                                }
                                else
                                {
                                    pos = Hash(auxDest2->code, hashAirports->size);
                                    auxAir3 = hashAirports->list[pos];
                                    sum+=auxDest2->price;
                                    strcpy(flight2, auxDest2->code);
                                    count++;
                                    while (auxAir3)
                                    {
                                        if (strcasecmp(auxAir3->code, auxDest2->code)==0)
                                        {
                                            auxDest3=auxAir3->listDestinations;
                                            while (auxDest3)
                                            {
                                                if (strcasecmp(auxDest3->code, destination)==0)
                                                {
                                                    sum+=auxDest3->price;
                                                    strcpy(flight3, auxDest3->code);
                                                    possible=true;
                                                    count++;
                                                }
                                                else
                                                {
                                                    sum=0;
                                                    strcpy(flight3, "0");
                                                }
                                                auxDest3=auxDest3->next;
                                            }
                                        }
                                        auxAir3=auxAir3->next;
                                    } 
                                }
                                auxDest2=auxDest2->next;
                            }
                        }
                        auxAir2=auxAir2->next;
                    }
                }
                auxDest=auxDest->next;
                if (sum>0 && sum<smallest)
                {
                    smallest=sum;
                    strcpy(winner1,flight1);
                    strcpy(winner2,flight2);
                    strcpy(winner3,flight3);
                }
            }
        }
        auxAir=auxAir->next;
    }
    if (possible)
    {
        printf("%f %s_%s_%s_%s\n", smallest, origin, winner1, winner2, winner3);
    }
    else
    {
        printf("No flights available!\n");
    }
}

//6º-Procedimento para determinar a melhor opção de voo entre duas cidades
void best_flight_option_city(HashFlights* hashFlights, HashAirports*hashAirports)
{
    char origin[10], destination[10], flight1[10], flight2[10], flight3[10], winner1[10], winner2[10], winner3[10];
    float sum=0, sum1=0, sum2=0, smallest=200000;
    int pos1, pos2, count=0, bcount=0;
    ListAirports* auxAir, *auxAir2, *auxAir3, *aux;
    ListDestinations* auxDest, *auxDest2, *auxDest3;
    bool possible=true, equal;

    fflush(stdin);
    printf("City of departure: ");
    scanf("%[^\n]s", origin);
    fflush(stdin);
    printf("City of arrival: ");
    scanf("%[^\n]s", destination);

    for (int i = 0; i < hashAirports->size; i++)
    {
        auxAir=hashAirports->list[i];
        while (auxAir)
        {
            if (strcasecmp(auxAir->city, origin)==0)
            {
                auxDest=auxAir->listDestinations;
                possible=false;
                while (auxDest)
                {
                    sum=0;
                    pos1 = Hash(auxDest->code, hashAirports->size);
                    aux = hashAirports->list[pos1];
                    if (strcasecmp(aux->city, destination)==0)
                    {
                        sum=auxDest->price;
                        possible=true;
                        count++;
                        strcpy(flight1,auxDest->code);
                        strcpy(flight2, "0");
                        strcpy(flight3, "0");
                    }
                    else
                    {
                        pos2 = Hash(auxDest->code, hashAirports->size);
                        auxAir2 = hashAirports->list[pos2];
                        sum+=auxDest->price;
                        strcpy(flight1, auxDest->code);
                        count++;
                        while (auxAir2)
                        {
                            if (strcasecmp(auxAir2->code, auxDest->code)==0)
                            {
                                auxDest2=auxAir2->listDestinations;
                                while (auxDest2)
                                {
                                    pos1 = Hash(auxDest2->code, hashAirports->size);
                                    aux = hashAirports->list[pos1];
                                    if (strcasecmp(aux->city, destination)==0)
                                    {
                                        sum+=auxDest2->price;
                                        possible=true;
                                        count++;
                                        strcpy(flight2, auxDest2->code);
                                        strcpy(flight3, "0");
                                    }
                                    else
                                    {
                                        pos1 = Hash(auxDest2->code, hashAirports->size);
                                        auxAir3 = hashAirports->list[pos1];
                                        sum+=auxDest2->price;
                                        strcpy(flight2, auxDest2->code);
                                        count++;
                                        while (auxAir3)
                                        {
                                            if (strcasecmp(auxAir->code, auxDest2->code)==0)
                                            {
                                                auxDest3=auxAir3->listDestinations;
                                                while (auxDest3)
                                                {
                                                    pos1= Hash(auxDest3->code, hashAirports->size);
                                                    aux = hashAirports->list[pos1];
                                                    if (strcasecmp(aux->city, destination)==0)
                                                    {
                                                        sum+=auxDest3->price;
                                                        strcpy(flight3, auxDest3->code);
                                                        possible=true;
                                                        count++;
                                                    }
                                                    else
                                                    {
                                                        sum=0;
                                                        strcpy(flight3, "0");
                                                    }
                                                    auxDest3=auxDest3->next;
                                                }
                                            }
                                            auxAir3=auxAir3->next;
                                        } 
                                    }
                                    auxDest2=auxDest2->next;
                                }
                            }
                            auxAir2=auxAir2->next;
                        }
                        auxDest=auxDest->next;
                        if (sum>0 && sum<smallest)
                        {
                            smallest=sum;
                            strcpy(winner1,flight1);
                            strcpy(winner2,flight2);
                            strcpy(winner3,flight3);
                        }
                    }
                }
            }
            auxAir=auxAir->next;
        }
    }
    if (possible)
    {
        printf("%f %s_%s_%s_%s\n", smallest, origin, winner1, winner2, winner3);
    }
    else
    {
        printf("No flights available!\n");
    }
}

#pragma endregion

#pragma region outros métodos

//Procedimento para alterar a distância e duração de uma certa ligação
void alter_distance_duration(HashFlights* hash)
{
    char input1[5], input2[5];
    float distance, duration;
    printf("Enter the aiport of departure(code): ");
    scanf("%[^\n]s", input1);
    fflush(stdin);
    printf("Enter the airports of arrival(code): ");
    scanf("%[^\n]s", input2);

    printf("%s\n", input1);
    printf("%s\n", input2);

    printf("New duration: ");
    scanf("%f", &duration);
    printf("New distance: ");
    scanf("%f", &distance);

    int pos = Hash(input1, hash->size);

    ListFlights* aux=hash->list[pos];
    
    while (aux!=NULL)
    {
        if (strcasecmp(aux->destination,input2)==0 &&strcasecmp(aux->origin,input1)==0)
        {
            aux->distance=distance;
            aux->duration=duration;
            aux->cost= calc_price(aux->type, duration, distance);
        }
        aux=aux->next;
    }
}

//Função para remover aeroporto de uma lista
ListAirports* remove_func_airports(ListAirports* lst, char input[10])
{
    if (lst)
    {
        if (strcasecmp(lst->code, input)==0)
        {
            ListAirports* next = lst->next;
            free(lst);
            lst=next;
        }
        else lst->next = remove_func_airports(lst->next, input);
    }
    return lst;
}

//Procedimento para remover aeroporto numa cidade específica
void remove_or_add_airports(HashAirports* hash)
{
    char city[50], country[50], code[10];
    int choice;

    printf("Do you want to remove or add an airport?: \n");
    printf("1-Add;\n2-Remove;\n0-Exit\n");
    printf("Choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            fflush(stdin);
            printf("Enter the code of the new airport: ");
            scanf("%[^\n]s", code);
            fflush(stdin);
            printf("Enter the name of the country: ");
            scanf("%[^\n]s", country);
            fflush(stdin);
            printf("Enter the name of the city: ");
            scanf("%[^\n]s", city);

            insert_hash_airports(hash, city, country, code);
            break;
        case 2:
            fflush(stdin);
            printf("Enter the code of the airport you want to remove: ");
            scanf("%[^\n]s", code);

            int pos = Hash(code, hash->size);

            ListAirports* aux=hash->list[pos];

            hash->list[pos] = remove_func_airports(hash->list[pos], code);
            break;
        default:
            break;
    }
}

//Procedimento para adicionar ligação entre dois aeroportos
void add_connection(HashFlights* hash, HashAirports* hashAirports)
{
    char origin[10], destination[10], type[50];
    float duration, distance;

    printf("Airport of departure for the flight: ");
    scanf("%[^\n]s", origin);
    fflush(stdin);
    printf("Airport of arrival for the flight: ");
    scanf("%[^\n]s", destination);
    fflush(stdin);
    printf("Duration: ");
    scanf("%f", &duration);
    printf("Distance: ");
    scanf("%f", &distance);
    fflush(stdin);
    printf("Type of flight (lowcost/normal): ");
    scanf("%[^\n]s", type);
    
    insert_hash_flights(hash, hashAirports,origin, distance, duration, type, destination);

}

//Procedimento para adicionar uma nova cidade
void add_city(HashAirports* hash)
{
    char city[50], country[50];

    fflush(stdin);
    printf("Name of the city: ");
    scanf("%[^\n]s", city);
    fflush(stdin);
    printf("Name of the country: ");
    scanf("%[^\n]s", country);

    insert_hash_airports(hash, city, country, "000");
}

#pragma endregion

#pragma region MENU e INFO

//Informações sobre como utilizar a aplicação
void Info()
{
    printf("=============MENU============================================\n");
    printf("1-Every city you can go to coming from a certain city;\n");
    printf("2-The city with the most flights out to different cities;\n");
    printf("3-The city with the most flights in from different cities;\n");
    printf("4-The most international airport of a given country;\n");
    printf("5-The best flight option between two airports;\n");
    printf("6-The best flight option between two cities;\n");
    printf("=============EXTRAS==========================================\n");
    printf("7-Print the airports Hash Table;\n");
    printf("8-Print the flights Hash Table;\n");
    printf("9-Alter the distance and duration of a flight;\n");
    printf("10-Add or remove an airport;\n");
    printf("11-Add a new connection between 2 airports;\n");
    printf("12-Add a new city;\n");
    printf("0-Exit.\n");
}

//Menu inicial
void Menu(HashAirports* hashAirports, HashFlights* hashFlights)
{
    int choice;

    Info();
    do
    {
        printf("Choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            fflush(stdin);
            possible_destinations(hashFlights, hashAirports);
            break;
        case 2:
            fflush(stdin);
            city_most_different_flights_from(hashFlights, hashAirports);
            break;
        case 3:
            fflush(stdin);
            city_most_different_flights_to(hashFlights, hashAirports);
            break;
        case 4:
            fflush(stdin);
            most_international_airport(hashAirports);
            break;
        case 5:
            fflush(stdin);
            best_flight_option_airports(hashFlights, hashAirports);
            break;
        case 6: 
            fflush(stdin);
            best_flight_option_city(hashFlights, hashAirports);
            break;
        case 7:
            fflush(stdin);
            hashAirports_visit(hashAirports);
            break;
        case 8:
            fflush(stdin);
            hashFlights_visit(hashFlights);
            break;
        case 9:
            fflush(stdin);
            alter_distance_duration(hashFlights);
            break;
        case 10:
            fflush(stdin);
            remove_or_add_airports(hashAirports);
            break;
        case 11:
            fflush(stdin);
            add_connection(hashFlights, hashAirports);
            break;
        case 12:
            fflush(stdin);
            add_city(hashAirports);
            break;
        default:
            break;
        }
    } while (choice!=0);
}

#pragma endregion

int main(int argc, char const *argv[])
{
    HashAirports* hashAirports = create_hash_airports(SIZEAir);
    HashFlights* hashFlights = create_hash_flights(SIZEfly);
    ListDestinations* listDestinations= NULL;
    
    read_file_airports(hashAirports);
    read_file_flights(hashFlights, hashAirports);
    Menu(hashAirports, hashFlights);

    return 0;
}