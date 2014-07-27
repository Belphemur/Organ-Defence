#!/bin/bash

choix=''
email=''
pause=''
save=`mv ./data/testWeapon.w ./concours/`

echo -e "Concours de création d'armes\n\n\n"
read pause
nano ./data/testWeapon.w
while [ -z $choix || $choix != '3' ]
do
    echo -e "1) Tester votre arme dans le jeu\n"
    echo -e "2) Enregistrer votre participation au concours\n"
    echo -e "3) Modifier votre arme\n"
    echo -e "4) Quitter sans rien faire" 

    if [ $choix = '1' ] 
    then
        ./organe_defense
    elif [ $choix = '2' ]
    then
        echo "Entrez votre adresse e-mail (pour vous contacter si vous gagnez)"
        read email
        save="mv ./data/testWeapon.w ./concours/$email.w"
        echo `save`
    elif [ $choix = '3' ]
    then
        nano ./data/testWeapon.w
    fi
done
