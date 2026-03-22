#include <stdio.h>
#include <stdlib.h> 
#include <strings.h>
#include <math.h> 
#include "lodepng.h" 

// принимаем на вход: имя файла, указатели на int для хранения прочитанной ширины и высоты картинки
// возвращаем указатель на выделенную память для хранения картинки
// Если память выделить не смогли, отдаем нулевой указатель и пишем сообщение об ошибке
unsigned char* load_png(const char* filename, unsigned int* width, unsigned int* height) 
{
  unsigned char* image = NULL; 
  int error = lodepng_decode32_file(&image, width, height, filename);
  if(error != 0) {
    printf("error %u: %s\n", error, lodepng_error_text(error)); 
  }
  return (image);
}

// принимаем на вход: имя файла для записи, указатель на массив пикселей,  ширину и высоту картинки
// Если преобразовать массив в картинку или сохранить не смогли,  пишем сообщение об ошибке
void write_png(const char* filename, const unsigned char* image, unsigned width, unsigned height)
{
  unsigned char* png;
  long long unsigned int pngsize;
  int error = lodepng_encode32(&png, &pngsize, image, width, height);
  if(error == 0) {
      lodepng_save_file(png, pngsize, filename);
  } else { 
    printf("error %u: %s\n", error, lodepng_error_text(error));
  }
  free(png);
}


// вариант огрубления серого цвета в ЧБ 
void contrast(unsigned char *col, int bw_size)
{ 
    int i, count = 0; 
    for(i=0; i < bw_size; i++)
    {
        if(col[i] < 60)
        col[i] = 0; 
        else {
          col[i] = 255;
        }
    } 
} 

// Гауссово размыттие
void Gauss_blur(unsigned char *col, unsigned char *blr_pic, int width, int height)
{ 
    int i, j; 
    for(i=1; i < height-1; i++) 
        for(j=1; j < width-1; j++)
        { 
            blr_pic[width*i+j] = 0.084*col[width*i+j] + 0.084*col[width*(i+1)+j] + 0.084*col[width*(i-1)+j]; 
            blr_pic[width*i+j] = blr_pic[width*i+j] + 0.084*col[width*i+(j+1)] + 0.084*col[width*i+(j-1)]; 
            blr_pic[width*i+j] = blr_pic[width*i+j] + 0.063*col[width*(i+1)+(j+1)] + 0.063*col[width*(i+1)+(j-1)]; 
            blr_pic[width*i+j] = blr_pic[width*i+j] + 0.063*col[width*(i-1)+(j+1)] + 0.063*col[width*(i-1)+(j-1)]; 
        } 
   return; 
} 

//  Место для экспериментов
void color(unsigned char *bw_pic, unsigned char *res, int size)
{ 
  int i;
    for(i=0;i<size;i++) 
    { 
        res[i*4]=bw_pic[i]; 
        res[i*4+1]=bw_pic[i]; 
        res[i*4+2]=bw_pic[i]; 
        res[i*4+3]=255; 
    } 
    return; 
}

int cnt(unsigned char* pic, int bw_size) {
  int count = 0;
  for(int i = 0; i < bw_size; i++) {
    if(pic[i] == 255) count++;
  }

  return count;
}

unsigned char rgb_to_bw(unsigned char r, unsigned char g, unsigned char b)
{
    return (unsigned char)((r + g + b) / 3);
}
  
int main() 
{ 
    const char* filename = "image.png"; 
    unsigned int width, height;
    int size;
    int bw_size;
    
    // Прочитали картинку
    unsigned char* picture = load_png("image.png", &width, &height); 
    if (picture == NULL)
    { 
        printf("Problem reading picture from the file %s. Error.\n", filename); 
        return -1; 
    } 

    size = width * height * 4;
    bw_size = width * height;
    
    
    unsigned char* bw_pic = (unsigned char*)malloc(bw_size*sizeof(unsigned char));
    unsigned char* blr_pic = (unsigned char*)malloc(bw_size*sizeof(unsigned char)); 
    unsigned char* finish = (unsigned char*)malloc(size*sizeof(unsigned char)); 
    
    for(int i = 0; i < bw_size; i++) {
      bw_pic[i] = rgb_to_bw(picture[4 * i], picture[4 * i + 1], picture[4 * i + 2]);
    }

    color(bw_pic, finish, bw_size); //* bw записываем в rgb

    //contrast(bw_pic, bw_size); 

    write_png("contrast.png", finish, width, height);

    //double count = cnt(bw_pic, bw_size) / 3.0;

    //printf("total ships = %.0lf\n", count);
    
    free(bw_pic);
    free(blr_pic); 
    free(finish);
    free(picture); 
    
    return 0; 
}
