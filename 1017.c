#include <stdio.h>
 
int main() {
 

    float km_por_litro = 12.0; 
    
  
    int horas_viagem;    
    int velocidade_media; 
    
  
    float litros_necessarios;
    
   
    float distancia_total;
 
    
    scanf("%d", &horas_viagem);
    scanf("%d", &velocidade_media);
 
    
    distancia_total = (float)horas_viagem * velocidade_media;
 
    
    litros_necessarios = distancia_total / km_por_litro;
 
   
    printf("%.3f\n", litros_necessarios);
 
    return 0;
}