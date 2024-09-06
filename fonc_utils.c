#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "IP_CIDR.h"


char* to_binary(int a) 
{
    int i;
    char* binary_str = (char*)malloc(9);
    binary_str[8] = '\0';

    for (i = 7; i >= 0; i--) 
    {
        binary_str[i] = (a % 2) ? '1' : '0';
        a /= 2;
    }

    return binary_str;
}

int binary_to_decimal(const char* binary_str) 
{
    int decimal = 0;
    int length = strlen(binary_str);
    int i;

    for ( i = 0; i < length; i++) 
    {
        if (binary_str[i] == '1') 
        {
            decimal += pow(2, length - i - 1);
        }
    }

    return decimal;
}

void verify_ip(int a, int b, int c, int d, int x) 
{
    if (a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255 ||
         x < 0 || x > 32) 
    {
        printf("\n\tAdresse IP invalide !");
        exit(1);
    }
}
char* msr(int x) 
{
    char *result = (char *)malloc(35 * sizeof(char));
    char temp[33];
    int i;
    for (i = 0; i < 32; i++) 
    {
        temp[i] = (i < x) ? '1' : '0'; 
    }
    temp[32] = '\0';

    snprintf(result, 35, "%.*s.%.*s.%.*s.%.*s",
            8, temp, 8, temp + 8, 8, temp + 16,
            8, temp + 24);

    return result;
}
void print_msr(int x)
{
    char *resultat = msr(x);
    char a_msr[9], b_msr[9],
         c_msr[9], d_msr[9];

    sscanf(resultat, "%8[^.].%8[^.].%8[^.].%8s", a_msr, b_msr, c_msr, d_msr);
    printf("%d.%d.%d.%d\n",
            binary_to_decimal(a_msr),
            binary_to_decimal(b_msr),
            binary_to_decimal(c_msr),
            binary_to_decimal(d_msr));
    free(resultat);
}


int nb_hote(int x) {
    if (x < 0 || x > 32) {
        fprintf(stderr, "Masque de sous-réseau invalide.\n");
        exit(1);
    }
    int bits_hotes = 32 - x;
    int nb_total = (int)pow(2, bits_hotes) - 2;

    return nb_total;
}

void calculate_network_address(int a, int b, int c, int d, int x) {
    int mask[4];
    int ip[4] = {a, b, c, d};
    int mask_bits = x;
    int i;

    for ( i = 0; i < 4; i++) {
        if (mask_bits >= 8) 
        {
            mask[i] = 255;
            mask_bits -= 8;
        } 
        else 
        {
            mask[i] = (255 << (8 - mask_bits)) & 255;
            mask_bits = 0;
        }
    }

    printf("Adresse réseau : %d.%d.%d.%d\n",
           ip[0] & mask[0],
           ip[1] & mask[1],
           ip[2] & mask[2],
           ip[3] & mask[3]);
}

void calculate_broadcast_address(int a, int b, int c, int d, int x) 
{
    int mask[4];
    int ip[4] = {a, b, c, d};
    int mask_bits = x;
    int i;

    for (i = 0; i < 4; i++) 
    {
        if (mask_bits >= 8) 
        {
            mask[i] = 255;
            mask_bits -= 8;
        } 
        else 
        {
            mask[i] = (255 << (8 - mask_bits)) & 255;
            mask_bits = 0;
        }
    }

    // Inversion du masque
    int inverse_mask[4];
    for (i = 0; i < 4; i++) 
    {
        inverse_mask[i] = ~mask[i] & 255;
    }

    printf("Adresse de diffusion : %d.%d.%d.%d\n",
           ip[0] | inverse_mask[0],
           ip[1] | inverse_mask[1],
           ip[2] | inverse_mask[2],
           ip[3] | inverse_mask[3]);
}

void gestion_ip() 
{
    int a, b, c, d, x;
    int nb_pc;
    char temp[32];
    printf("Entrez l'adresse IP : (ex : X.X.X.X / X)\n\t");
    if (scanf("%31s", temp) != 1) 
    {
        printf("Erreur de lecture de l'adresse IP\n");
        exit(1);
    }

    if (sscanf(temp, "%d.%d.%d.%d/%d", &a, &b, &c, &d, &x) != 5) 
    {
        printf("Erreur de format de l'adresse IP/CIDR\n");
        exit(1);
    }

    verify_ip(a, b, c, d, x);

    printf("Masque de Sous-Reseau : ");
    print_msr(x);

    calculate_network_address(a, b, c, d, x);
    calculate_broadcast_address(a, b, c, d, x);

    nb_pc = nb_hote(x);
    printf("Nombres de PC : %d\n", nb_pc);
}
