 
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
 
typedef unsigned char u8;
typedef unsigned int  u32;
 
static int file_read_bin_data(const char *file, u8 *buf, int size)  
{  
    FILE *fp;  
      
    if((fp=fopen(file, "rb")) == NULL) {  
        printf("Can not open the file: %s \n", file);  
        return -1;  
    }  
	
    fread(buf, sizeof(u8), size, fp);
	
    fclose(fp); 
 
	return 0;	
} 
 
static u32 file_get_bin_size(const char *file)  
{     
    u32  size = 0;     
    FILE  *fp = fopen(file, "rb");     
    if (fp) {        
        fseek(fp, 0, SEEK_END);        
        size = ftell(fp);        
        fclose(fp);     
    }     
    return size;  
} 
 
int file_out_handle(const char *array_name, const char *out_file, u8 *buf, u32 size, u32 bmp_size)  
{  
    FILE *fp; 
    int i,j,k,n;  
    int fd ;
    char pbuf[10]={0};  
    char mfgimage[4096*2];
 
    if((fp=fopen(out_file, "wa+"))==NULL) {  
        printf( "\nCan not open the path: %s \n", out_file);  
        return -1;  
    }  
    k=0;
    char title_str[512] = {0};

    snprintf(title_str, sizeof(title_str), "#define BMP_IMAGE_SIZE %d\n\n", bmp_size);
    fwrite(title_str, strlen(title_str), 1, fp);
    snprintf(title_str, sizeof(title_str), "static const uint8_t %s[] = {\n    ", array_name);
 	fwrite(title_str, strlen(title_str), 1, fp); 
    for(i = 0; i < size; i++) {  
		k++;  
		sprintf(pbuf,"0x%02x", buf[i]);  
		fwrite(pbuf,strlen(pbuf),1,fp); 
		
		if(k != 16)  
			fwrite(", ", strlen(", "),1,fp);  
		else  
			fwrite(",", strlen(","),1,fp);
		
		if(k == 16) {  
			k=0;  
			fwrite("\n    ", strlen("\n    "),1,fp);  
		}  
    }  
	
	fwrite("\n};\n;", strlen("\n};\n"),1,fp);
    
    fclose(fp); 
	
	return 0;
}   
 
int main(int argc, const char *argv[])  
{ 
	u8 *buf = NULL;  
	u32 size;  
	const char *src_bin = NULL;  
	const char *dst_file= NULL; 
	const char *array_name = NULL;
	int bmp_size = 0;
 
	if (argc != 5) {
		printf("Error param input !\r\n");
		printf("Usage: %s src_bin out_file array_name BMP_SIZE_ONE_FILE\e\n", argv[0]);
		exit(1);
	}
	
	src_bin = (const char *)argv[1];
	dst_file = (const char *)argv[2];
	array_name = (const char *)argv[3];
	bmp_size = atoi(argv[4]);
	size = file_get_bin_size(src_bin);
	
	buf = (u8 *)malloc(sizeof(u8) * size); 
	if (buf) {	
		file_read_bin_data(src_bin, buf, size);		
		file_out_handle(array_name, dst_file, buf, size, bmp_size);		
		free(buf);
	}
	
    return 0;  
}  