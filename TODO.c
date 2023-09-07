/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TODO.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbily <fbily@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/07 13:32:43 by fbily             #+#    #+#             */
/*   Updated: 2023/09/07 16:25:16 by fbily            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/* 
Fonctions a faire :
	- JOIN 			<<<< Gerer les exception + different reply
	- Invite
	- Kick
	- Topic
	- Mode :
				-i (set/unset invite only)
				-t ()
				-k (set/unset key)
				-o (Give/Take operator privilege)
				-l (set/unset user limit)
	- Map dans serveur.cpp => std::map<string, channel*> [name] = *channel;
	
A savoir :
	- Recoit on les user* a inviter ? Ex : Zak invite Flo sur channel 42 => On recoit Zak mais Flo ?
	- Variable par default pour creation channel

/!\ Savoir pour les leaks Channel en fin d'exec /!\
/!\ Supprimer des channel user qui quitte son client. /!\

Messages to add :

	- RPL_TOPICWHOTIME				<<<<< TOPIC (+ JOIN ?)

	- RPL_UMODEIS
	- ERR_UMODEUNKNOWFLAG
 */