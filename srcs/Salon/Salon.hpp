#ifndef SALON_HPP
# define SALON_HPP

#include "../../includes/inc.hpp"
#include "../Client/Client.hpp"

//classe salon
//-> doit avoir plusieurs clients qui pourront s'envoyer des messages
// le salon doit avoir un nom
// -> mettre a jours les clients quand ils se connectent


class Salon
{
	public:
		Salon(const Salon &src);
		Salon& operator=(Salon const &rhs);
		~Salon();

        Salon(std::string name);
        std::string getName();
        void setName(std::string name);
		int getNumberOfClient();
        void showMessage();

	private:
		std::string		_Name;			// name du Salon
	    int  			_numberOfClient;		// nombre de client
		Salon();
};

#endif