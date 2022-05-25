/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tglory <tglory@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 19:18:25 by tglory            #+#    #+#             */
/*   Updated: 2022/05/25 15:00:09 by tglory           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <unistd.h>
#include <string>
#define LG_MESSAGE 256

int main() {
	int descripteurSocket;
	struct sockaddr_in pointDeRencontreDistant;
	socklen_t longueurAdresse;
	char messageRecu[LG_MESSAGE];
	int ecrits, lus;

	descripteurSocket = socket(PF_INET, SOCK_STREAM, 0); 

	if (descripteurSocket < 0) {
		perror("socket"); 
		exit(-1); 
	}
	printf("Socket créée avec succès ! (%d)\n", descripteurSocket);
	
	longueurAdresse = sizeof(pointDeRencontreDistant);
	
	memset(&pointDeRencontreDistant, 0x00, longueurAdresse);
	
	pointDeRencontreDistant.sin_family = PF_INET;
	
	pointDeRencontreDistant.sin_port = htons(6667); 
	
	inet_aton("127.0.0.1", &pointDeRencontreDistant.sin_addr); 
	
	if (connect(descripteurSocket, (struct sockaddr *)&pointDeRencontreDistant, longueurAdresse) == -1) {
		perror("connect"); 
		close(descripteurSocket); 
		exit(-2); 
	}
	printf("Connexion au serveur réussie avec succès !\n");
	
	
	memset(messageRecu, 0x00, LG_MESSAGE*sizeof(char));

	std::string cmd = "PASS password\r\n";
	ecrits = send(descripteurSocket, cmd.c_str(), cmd.length(), 0); 
	switch(ecrits)
	{
		case -1 :
			perror("write");
			close(descripteurSocket);
			exit(-3);
		case 0 :
			fprintf(stderr, "La socket a été fermée par le serveur !\n");
			close(descripteurSocket);
			return 0;
		default: 
			printf("Message '%s' envoyé avec succès (%d octets)\n", cmd.c_str(), ecrits);
	}

	lus = read(descripteurSocket, messageRecu, LG_MESSAGE*sizeof(char));
	switch(lus)
	{
		case -1 : 
			perror("read");
			close(descripteurSocket);
			exit(-4);
		case 0 : 
			fprintf(stderr, "La socket a été fermée par le serveur !\n\n");
			close(descripteurSocket);
			return 0;
		default: 
			printf("Message reçu du serveur : %s (%d octets)\n\n", messageRecu, lus);
	}
	close(descripteurSocket);
	return 0;
}
