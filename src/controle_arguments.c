#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <structure.h>
#include <downsampling.h>
#include <controle_arguments.h>

enum argument{
    HELP,
    SAMPLE,
    OUTFILE,
    FICHIER_PPM,
    NB_ARGS
};

uint8_t* controle_arg(int argc, char **argv){
    if(argc < 2 || argc > 7){
        printf("Erreur : le nombre d'arguments n'est pas correct\n");
        exit( EXIT_FAILURE);
    }
    uint8_t arg_i = 1;
    uint8_t argument;
    uint8_t* pos_des_param = calloc(0, sizeof(uint8_t));
    uint8_t main_arg = 0;
    while(arg_i < argc){
    //Étape de reconnaissance :
        if(strcmp(argv[arg_i], "--help" ) == 0){
            arg_i ++;
            argument = 0;
        }
        else if(strcmp(argv[arg_i], "--sample" ) == 0){
            arg_i ++;
            argument = 1;
        }
        else if(strcmp(argv[arg_i], "--outfile" ) == 0){
            arg_i ++;
            argument = 2;
        }
        else{
            //Analyse du paramètre principal
            main_arg ++;
            FILE *file = fopen(argv[arg_i], "rb");
            fclose(file);
            argument = 3;
        }

        switch(argument){
            case HELP:
                printf("Liste des arguments possibles : \n  --help : pour afficher la sortie des options acceptées\n  --outfile sortie.jpg : pour redéfinir la sortie.jpg\n  --sample h1xv1,h2xv2,h3xv3 : pour redéfinir les facteurs hi x vi des trois composantes de couleurs \n\nLa fonction ne sera pas exécutée après l'argument help passé en paramètre\n");
                exit(EXIT_SUCCESS);
                break;
            case SAMPLE:
                pos_des_param[0] = arg_i;
                arg_i ++;
                break;
            case OUTFILE:
                pos_des_param[1] = arg_i;
                arg_i ++;
                break;
            case FICHIER_PPM:
                pos_des_param[2] = arg_i;
                arg_i ++;
                break;
            default:
                printf("L'argument : %u  est invalide.\n", arg_i +1);
                exit(EXIT_FAILURE);
        }
        if(main_arg == 0){
            //Cas où l'on a pas trouvé l'argument principal
            printf("Veuillez rentrez un fichier ppm/pgm en argument\n");
            exit(EXIT_FAILURE);
        }
    }
    return pos_des_param;
}

void analyse_arg_sub(uint8_t* pos_des_arguments, struct main_mcu_rgb_sub* p_main, char** argv){
    /*Dans cet fonction, on analyse les options rentrées dans les arguments et on modifie notre structure principale en conséquence*/
    
    //On commence par analyser les sampling_factor
    if(pos_des_arguments[0] == 0){
        //Cas où l'utilisateur n'a pas rentré de sampling factor, on prend donc par défaut une image sans subsampling:
        p_main->sampling_factor[0]= 1;
        p_main->sampling_factor[1]= 1;
        p_main->sampling_factor[2]= 1;
        p_main->sampling_factor[3]= 1;
        p_main->sampling_factor[4]= 1;
        p_main->sampling_factor[5]= 1;
    }
    else{
        //On commence par vérifier que la taille de l'argument est bonne (on suppose que hi et vi ne sont que des chiffres)
        if(strlen(argv[pos_des_arguments[0]]) != 11){
            printf("L'argument des sampling factor n'a pas la bonne taille.\nAssurez vous des prendre des facteurs qu'à 1 chiffre.\n\nL'encodage se fera donc sans sous-échantillonage.\n");
            p_main->sampling_factor[0]= 1;
            p_main->sampling_factor[1]= 1;
            p_main->sampling_factor[2]= 1;
            p_main->sampling_factor[3]= 1;
            p_main->sampling_factor[4]= 1;
            p_main->sampling_factor[5]= 1;
        }
        else{
            uint8_t pos = pos_des_arguments[0];
            if(isdigit(argv[pos][0])!=0 && isdigit(argv[pos][2])!=0 && isdigit(argv[pos][4])!=0 && isdigit(argv[pos][6])!=0 && isdigit(argv[pos][8])!=0 && isdigit(argv[pos][10])!=0){
                //Cas où tout est correcte
                p_main->sampling_factor[0] = atoi(&(argv[pos][0]));
                p_main->sampling_factor[1] = atoi(&(argv[pos][2]));
                p_main->sampling_factor[2] = atoi(&(argv[pos][4]));
                p_main->sampling_factor[3] = atoi(&(argv[pos][6]));
                p_main->sampling_factor[4] = atoi(&(argv[pos][8]));
                p_main->sampling_factor[5] = atoi(&(argv[pos][10]));
                //On finit par contrôler les arguments : 
                verif_sample_factor(p_main);
            }
            else{
                printf("L'argument des sampling factor n'a pas la bonne taille.\nAssurez vous des prendre des facteurs qu'à 1 chiffre.\n\nL'encodage se fera donc sans sous-échantillonage.\n");
                p_main->sampling_factor[0]= 1;
                p_main->sampling_factor[1]= 1;
                p_main->sampling_factor[2]= 1;
                p_main->sampling_factor[3]= 1;
                p_main->sampling_factor[4]= 1;
                p_main->sampling_factor[5]= 1;
            }
        }        
    }
    // On stocke le nom du fichier ppm, on suppose qu'il est bon
    p_main->ppm_filename = argv[pos_des_arguments[2]];

    //Puis on traite le fichier jpeg
    if(pos_des_arguments[1] == 0){
        p_main->jpeg_filename = "mon_test.jpg";
    }
    else{
        if(argv[pos_des_arguments[1]][0] == *("~")){
            char* home_path = getenv("HOMEPATH");
            argv[pos_des_arguments[1]][0] = *("");
            p_main->jpeg_filename = strcat(home_path, argv[pos_des_arguments[1]]);
        }
        else{
            p_main->jpeg_filename = argv[pos_des_arguments[1]];
        }
    }
}

void analyse_arg(uint8_t* pos_des_arguments, struct main_mcu* p_main, char** argv){
    /*Dans cet fonction, on analyse les options rentrées dans les arguments et on modifie notre structure principale en conséquence*/
    
    //On commence par analyser les sampling_factor
    if(pos_des_arguments[0] != 0){
        printf("Il n'y a pas de downsampling dans les images grises donc l'argument --sample n'est pas pris en compte.\n");
    }
    // On stocke le nom du fichier ppm, on suppose qu'il est bon
    p_main->ppm_filename = argv[pos_des_arguments[2]];

    //Puis on traite le fichier jpeg
    if(pos_des_arguments[1] == 0){

        
        p_main->jpeg_filename = "mon_test.jpg";
    }
    else{
        if(argv[pos_des_arguments[1]][0] == *("~")){
            char* home_path = getenv("HOMEPATH");
            argv[pos_des_arguments[1]][0] = *("");
            p_main->jpeg_filename = strcat(home_path, argv[pos_des_arguments[1]]);
        }
        else{
            p_main->jpeg_filename = argv[pos_des_arguments[1]];
        }
    }
}