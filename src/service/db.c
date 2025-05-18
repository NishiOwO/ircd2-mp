#include "os.h"
#include "mpserv.h"

FILE* dbopen(const char* path){
	FILE* f = fopen(path, "r+b");
	if(f != NULL) return f;

	f = fopen(path, "wb");
	if(f == NULL) return NULL;
	fclose(f);

	f = fopen(path, "r+b");
	return f;
}

void dbdel(FILE* f, const char* key){
	char s2[KEYSIZE + 1];
	unsigned char buf[VALSIZE + 2];
	if(strlen(key) > KEYSIZE) return;
	s2[KEYSIZE] = 0;
	fseek(f, 0, SEEK_SET);
	while(1){
		int a = 0;
		unsigned char n;
		if(fread(s2, 1, KEYSIZE, f) < KEYSIZE) break;

		if(strcmp(key, s2) == 0){
			unsigned char z[KEYSIZE];
			memset(z, 0, KEYSIZE);
			fseek(f, -KEYSIZE, SEEK_CUR);
			fwrite(z, 1, KEYSIZE, f);
			fflush(f);
			break;
		}
		fread(buf, 1, VALSIZE + 2, f);
	}
}

void dbset(FILE* f, const char* key, dbdata_t* val){
	char s[KEYSIZE + 1];
	unsigned char c;
	unsigned char buf[VALSIZE + 2];
	fseek(f, 0, SEEK_SET);
	while(1){
		int a = 0;
		unsigned char n;
		if(fread(s, 1, KEYSIZE, f) < KEYSIZE) break;
		if(s[0] == 0 || strcmp(s, key) == 0){
			fseek(f, -KEYSIZE, SEEK_CUR);
			break;
		}
		fread(buf, 1, VALSIZE + 2, f);
	}
	memset(s, 0, KEYSIZE + 1);
	strcpy(s, key);
	fwrite(s, KEYSIZE, 1, f);
	c = (val->size >> 8) & 0xff;
	fwrite(&c, 1, 1, f);
	c = val->size & 0xff;
	fwrite(&c, 1, 1, f);
	fwrite(val->value, 1, VALSIZE, f);
	fflush(f);
}

char* dbeach(FILE* f, dbdata_t* val){
	char s2[KEYSIZE + 1];
	unsigned char buf[VALSIZE + 2];
	s2[KEYSIZE] = 0;
	while(1){
		int a = 0;
		unsigned char n;
		if(fread(s2, 1, KEYSIZE, f) < KEYSIZE) break;
		if(s2[0] == 0){
			fread(buf, 1, VALSIZE + 2, f);
			continue;
		}

		fread(&n, 1, 1, f);
		a = a << 8;
		a = a | n;
		fread(&n, 1, 1, f);
		a = a << 8;
		a = a | n;

		val->size = a;
		fread(val->value, 1, VALSIZE, f);
		return copystr(s2);
	}
	return NULL;
}

int dbget(FILE* f, const char* key, dbdata_t* val){
	char s2[KEYSIZE + 1];
	unsigned char buf[VALSIZE];
	if(strlen(key) > KEYSIZE) return 0;
	s2[KEYSIZE] = 0;
	fseek(f, 0, SEEK_SET);
	while(1){
		int a = 0;
		unsigned char n;
		if(fread(s2, 1, KEYSIZE, f) < KEYSIZE) break;

		fread(&n, 1, 1, f);
		a = a << 8;
		a = a | n;
		fread(&n, 1, 1, f);
		a = a << 8;
		a = a | n;

		if(strcmp(key, s2) == 0){
			val->size = a;
			fread(val->value, 1, VALSIZE, f);
			return 1;
		}else{
			fread(buf, 1, VALSIZE, f);
		}
	}
	return 0;
}

void dbclose(FILE* f){
	fclose(f);
}
