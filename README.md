Les premières lignes du code importent les bibliothèques nécessaires pour le fonctionnement du programme, à savoir "WiFi.h" pour la connexion Wi-Fi, "PubSubClient.h" pour la communication MQTT, "SPI.h" pour la communication SPI et "LoRa.h" pour la communication LoRa.

Ensuite, il y a des définitions de constantes pour les broches utilisées pour la communication SPI avec le module LoRa.

Les lignes suivantes définissent les noms d'utilisateur et mot de passe du réseau Wi-Fi ainsi que le serveur MQTT utilisé.

La variable "verif" est définie pour savoir si la connexion LoRa a déjà été configurée ou non.

La structure de l'union "pack" est définie pour stocker les données de LoRa.

La fonction "setup_wifi()" est définie pour établir une connexion Wi-Fi.

La fonction "setup()" est appelée au début du programme pour initialiser les broches utilisées et la connexion MQTT.

La fonction "reconnect()" est utilisée pour reconnecter le client MQTT lorsque la connexion est perdue.

La fonction "callback()" est appelée lorsque le client MQTT reçoit un message et elle est utilisée pour extraire les données reçues.

La boucle "loop()" est exécutée en continu. Elle vérifie si le client MQTT est connecté et si un paquet LoRa est reçu. Si c'est le cas, elle extrait les données du paquet et les envoie au serveur MQTT.
