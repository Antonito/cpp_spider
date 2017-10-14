# cpp_spider
Ce document détaille le fonctionnement du projet _cpp\_spider_.

## Généralités
La communication client/serveur est chiffrée en TLSv1 grâce à la bibliothèque OpenSSL.
Cette communication est séparé en 2 canaux : un cannal dit de commandes, et un canal de données.

Les données échangées sur le canal de commandes sont des commandes textuelles, terminées par un CRLF. Les données échangés sur l'autre canal sont échangées dans un format binaire défini par notre logiciel.

Les victimes sont identifiés grâce à leurs adresses MAC.

## Protocole

## Client
Notre client est composé d'un noyau cross-plateform, et de modules propre à chaque plateforme ciblée. Les-dits modules sont présentés sous la forme d'un `.so` dans le cas de Linux, d'un `.dylib` dans le cas de MacOS, et d'un `.dll` sous Windows.

Lors du démarrage du client, le noyau (dit _core_) charge le module spécifique à la plateforme. Une fois ce chargement effectué, le core vérifie la présence de deboggeur, et vérifie que le processus n'est pas analysé par un anti-virus. Si le core n'est pas analysé, il procède ensuite à l'initialisation du module, puis à l'initialisation d'un thread de communication réseau. En cas d'erreur, le processus est arrêté.

Le thread de réseau essaie continuellement de se connecter au serveur, en espaçant chaque tentative échouée de 3 secondes. Une fois connecté, celui-ci est responsable de la transmission des ordres du serveur au core, et de l'envoi des informations envoyés par ce dernier. La communication inter-thread est effectuée au moyen de `queues` thread-safe.
Ainsi, si pour une raison la connexion réseau est interrompue, l'ensemble des évènements détectés reste sauvegardé dans une `queue`, et ces évènements seront par la suite envoyés au serveur dès que la connexion sera rétablie.

De son coté, le core effectue une boucle infinie répétant les étapes suivantes:

	- Vérification de la présence d'un deboggeur
	- Récupèration et traitement des commandes envoyés par le serveur
	- Appel de la méthode 'run' de module chargé

En cas de détection d'un deboggeur, le core s'arrête.

#### Module

Un module est une bibliothèque dynamique réalisée pour un système d'exploitation particulier.
Afin d'homogénéiser leur conception, les modules doivent implémenter le code suivant:

```cpp
static spider::client::library::SpiderPlugin payload;

extern "C" {
SPIDER_API spider::client::library::IPayload *getPayload()
{
  return static_cast<spider::client::library::IPayload *>(&payload);
}
```

Ils doivent également se conformer à l'interface `IPayload`.

#### Speficités Windows

Sous Windows, différentes protections sont mises en place.
La console d'exécution est cachée, on empêche le lancement de plus d'une instance du programme (via le système de mutex kernel), on supprime l'entête `PE` de l'exécutable, on enregistre le programme en tant que programme de démarrage (grâce à une entrée dans le registre), on change les droits DACL afin d'empêcher la fermeture de l'exécutable via le `Task Manager`, et on rend l'exécutable "critique" (sous certaines versions anciennes de Windows, une fermeture du programme entrainera un BSOD).

On désactive également la _croix_ permettant habituellement de fermer un processus. Ceci n'est utile que dans le cas exeptionnel où la console resterait visible.

Dans le cas d'un executable 32 bits, on cache également le thread (il ne sera pas visible via un deboggeur), et on falsifie la taille de l'exécutable mappé en mémoire (empêchant ainsi un dump correct de la mémoire).

Tout au long de l'exécution du virus, ce dernier cherche le processus dénommé `Task Manager` et se charge de le tuer si il est trouvé.

#### Speficités Linux

Aucune spécificité.

#### Speficités MacOS

Aucune spécificité. 


## Serveur

Notre serveur dispose également d'un système de plugin sous forme de bibliothèques dynamiques.
Au démarrage, nous chargeons un objet dénommé `CommandCenter`, qui s'occupe du chargement des différents plugins. 

![Architecture Serveur](./serveur_architecture.png)

Les clients du serveur peuvent être controlés par des éléments dénommés `Controllers`. Nous disposons actuellement de deux `Controllers`: un `Shell` et un `WebServer` (ce dernier met à disposition une API Restful).
L'ensemble de la communication interne s'effectue via un système de `queues`.

Un plugin peut être réalisé grâce aux informations disponibles dans un autre document (`../project/server_plugin/readme.md`).

Les plugins sont chargés d'effectuer les envois réseaux, et les envois de réponse aux controllers. Ceci est possible grâce à l'interface `IEventable`, directement inspirée des `IEquatable` et autres `protocols` disponibles dans la bibliothèque standart du Swift.