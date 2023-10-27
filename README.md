# Jiscord

## Description

**Jiscord** est une plateforme de discussion en temps réel développée par Victor, enseignant en programmation chez Junia, en collaboration avec ses étudiants. Cette application de messagerie est conçue pour offrir une expérience fluide et intuitive, avec un serveur robuste et un client léger et réactif. 

Le projet est divisé en deux parties majeures:
1. **Serveur**: Écrit en C, il est responsable de la gestion des messages, de la connexion des utilisateurs et de toutes les autres tâches de back-end.
2. **Client**: Le client combine une interface front-end HTML/CSS/JS avec une logique back-end en C++ en utilisant le framework Ultralight.

## Démarrage

### Client

Le client est basé sur le quickstart d'Ultralight. Assurez-vous d'avoir Ultralight et ses dépendances correctement installées.

1. Naviguez vers le dossier du client:
    ```bash
    cd client
    ```

2. Suivez les instructions de compilation.

   ```bash
    cmake -B build
    cmake --build build --config Release
    ```

3. Une fois compilé, lancez le client:
    ```bash
    ./build/Release/Jiscord.exe
    ```

## Comment contribuer au projet?

Si vous êtes étudiant chez Junia et souhaitez apporter votre pierre à l'édifice, voici comment procéder:

1. **Fork**: Commencez par créer un fork du dépôt.
2. **Clone**: Clonez votre fork sur votre machine locale.
3. **Branch**: Créez une nouvelle branche pour votre fonctionnalité ou correction.
4. **Code**: Apportez vos modifications ou ajouts.
5. **Commit**: Effectuez des commits clairs et précis.
6. **Push**: Poussez votre branche sur GitHub.
7. **Pull Request**: Créez une nouvelle Pull Request à partir de votre branche vers le dépôt original.

**Note**: Avant de soumettre votre Pull Request, assurez-vous que votre code est bien documenté et conforme aux standards de codage du projet.

## Conclusion

Un grand merci à tous les étudiants pour leur implication et leur passion dans ce projet. Nous sommes impatients de voir Jiscord grandir avec l'aide de la communauté. Si vous avez des questions ou des suggestions, n'hésitez pas à ouvrir une issue ou à contacter Victor directement.

Bonne programmation!
