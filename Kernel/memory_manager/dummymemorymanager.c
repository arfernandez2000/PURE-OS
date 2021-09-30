
char * ptr;

void meminit(void * startingPoint){

    ptr = (char *) startingPoint;
}

char * alloc(unsigned int bytes){
    char * aux = ptr;
    ptr += bytes;
    return aux;
}

