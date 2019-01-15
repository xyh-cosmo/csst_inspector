#include <stdio.h>
#include <stdlib.h>
#include "../iniparser/dictionary.h"
#include "../iniparser/iniparser.h"

int main( int argc, char* argv[] ){

//	load the input ini file
	dictionary *dict_ini = iniparser_load(argv[1]);

//	dump the loaded content into a temp.ini
	FILE *fp_dump_ini = fopen("temp_dump.ini","w");
	iniparser_dump_ini(dict_ini,fp_dump_ini);
	fclose(fp_dump_ini);

	int sec_num = iniparser_getnsec(dict_ini);
	int nkey[sec_num];

	// output all section names
	int i,j;
	int nkey_total=0;

	i = 0;
	while( i< sec_num ){
		const char *secname = iniparser_getsecname(dict_ini,i);
		nkey[i] = iniparser_getsecnkeys(dict_ini,secname);
		nkey_total = nkey[i] > nkey_total ? nkey[i] : nkey_total;
		i = i+1;
	}

	const char **sec_keys = (const char **)malloc(sizeof(char *)*nkey_total);
	for( j=0; j<nkey_total; j++ ){
		sec_keys[j] = (char *)malloc(sizeof(char)*1024);
	}

	i=0;
	while( i < sec_num ){
		const char *sec_name = iniparser_getsecname(dict_ini,i);
		printf("Section[%d] is: %s\n",i,sec_name);

		sec_keys = iniparser_getseckeys(dict_ini,sec_name,sec_keys);

		for(j=0; j<nkey[i]; j++ ){
			const char *value;
			value = iniparser_getstring(dict_ini,sec_keys[j],"");
			printf("%s : %s\n",sec_keys[j],value);
		}

		i = i+1;
		if( i < sec_num ) printf("\n");
	}

	for( j=0; j<nkey_total; j++ ){
		free((void*)sec_keys[j]);
	}
	free(sec_keys);

	iniparser_freedict(dict_ini);
}
