# Travaux à effectuer

## Kde Neon

error: CMake project configuration failed. No CMake configuration for build type "Release" found.
Solution:
sudo apt-get install qtbase5-dev qtdeclarative5-dev

## Plugin Moodle

* ~~Il faut dans un dossier EWB créer une extension pour Moodle 4.0.7 identique à "Safe Exam Browser" (https://docs.moodle.org/501/en/Safe_Exam_Browser) pour qu'il apparaisse comme une option dans les paramètres de test en ligne : etre disponible uniquement si ExamWebBrowser est utilisé.~~ **FAIT** — Plugin `quizaccess_ewb` dans `moodle-plugin/`

* ~~L'installation de l'extension sur le serveur Moodle 4.0.7 doit se faire par l'interface web d'administration avec une archive zip contenant le nécessaire.~~ **FAIT** — Archive `moodle-plugin/quizaccess_ewb.zip`

