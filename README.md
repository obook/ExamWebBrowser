<p align="center">
    <a href='https://www.smb33.fr/' target='_blank'><img src="./images/logo.svg"  width="120"></a>
</p>

Navigateur Web ultra simplifié avec mode examen pour l'Ensemble Scolaire Sainte-Marie Bastide à Bordeaux.
Il est utilisé avec le module de QCM en ligne Moodle (activité *test*) en salle informatique sous la surveillance de l'enseignant.

> [!CAUTION]
> Le programme  doit être lancé à partir d'un stockage local.
>
> Il ne fonctionne pas à partir d'un stockage réseau en raison de la sécurité de Windows sur le chargement des dll.

## ✨ Fonctionnalités principales

| | Fonctionnalité |
|---|---|
| 🖥️ | Une seule fenêtre en plein écran, pas de bouton de fermeture, aucun réglage possible |
| 👁️ | Détection de la perte de focus : le navigateur se bloque et nécessite l'appel du surveillant |
| 🔒 | URL pré-configurée, aucune sortie du domaine n'est autorisée |
| 🏠 | Icone pour revenir à l'URL d'accueil |
| 🕐 | Horloge permanente |
| 🚫 | Pas de barre d'adresse ni de recherche |
| 📄 | Pas de téléchargement de documents autres que HTML, CSS, JS et les images |
| ⌨️ | Raccourcis clavier désactivés |
| 🎓 | **Plugin Moodle** : restriction d'accès aux tests pour n'autoriser que ExamWebBrowser |

## 📦 Installation

Autonome (*standalone*), il ne nécessite aucune installation. Il peut être placé sur une clé USB, mais **pas sur un lecteur réseau Microsoft Windows** (*mapped drive* = dll non chargées).

## 🔧 Configuration

Vous pouvez configurer le navigateur en éditant le fichier `ExamWebBrowser.ini` créé au premier lancement.

La sortie du programme et l'entrée du code de déverrouillage se font respectivement par un clic droit et clic gauche sur l'horloge.

<p align="center">
    <a href='https://www.smb33.fr/' target='_blank'><img src="./images/ScreenShot.gif"  width="700"></a>
</p>

## 🎓 Plugin Moodle (quizaccess_ewb)

Un plugin Moodle de type *quiz access rule* est fourni dans `moodle-plugin/`. Il permet de restreindre l'accès à une activité *test* aux seuls élèves utilisant ExamWebBrowser, de manière similaire à Safe Exam Browser (SEB).

### ⚙️ Fonctionnement

Le navigateur ExamWebBrowser envoie un User-Agent contenant `ExamWebBrowser/1.0`. Le plugin Moodle vérifie la présence de cette chaîne et bloque l'accès au test si elle est absente. Les enseignants et administrateurs sont automatiquement exemptés de cette vérification.

### 📥 Installation sur Moodle 4.x

1. Récupérer l'archive `moodle-plugin/quizaccess_ewb.zip`
2. Dans Moodle, aller dans **Administration du site > Extensions > Installer des extensions**
3. Déposer l'archive zip et suivre les étapes de validation
4. Moodle installe le plugin et met à jour la base de données

### 🚀 Utilisation

1. Modifier un test (*quiz*) dans Moodle
2. Dans les paramètres du test, repérer l'option **Exiger ExamWebBrowser** et la mettre à **Oui**
3. 🚫 Les élèves avec un navigateur classique verront un message d'erreur
4. ✅ Les élèves utilisant ExamWebBrowser pourront passer le test normalement

## 🔨 Compilation

```bash
./build_Linux.sh
```

L'exécutable est généré dans `build/linux/`.

## ⬇️ Téléchargement

Les installateurs Linux (`.deb`) et Windows (`.zip`) sont générés automatiquement à chaque release via GitHub Actions.

Téléchargement depuis les **[Releases GitHub](https://github.com/obook/ExamWebBrowser/releases)**.

### Linux (.deb)

```bash
sudo dpkg -i examwebbrowser_*.deb
sudo apt-get -f install   # pour les dépendances Qt6
```

L'application est installée dans `/opt/examwebbrowser/` avec un lien `/usr/bin/examwebbrowser` et une entrée dans le menu applications.

### Windows (.zip)

Extraire l'archive et lancer `ExamWebBrowser.exe`. Toutes les DLLs Qt sont incluses, aucune installation requise.
