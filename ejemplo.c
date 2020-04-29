#include <stdio.h>

int main(int argc, void * argv[]){

	for(unsigned i = 0 ; i < argc ; i++){
		printf("%s", argv[i]);
	}

	return 0;
}
