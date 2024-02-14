#include <audioManager.h>
#include <iostream>

AudioManager manager;

void ThreadB()
{
	if (!manager.Initialize())
		std::cout << "Initialize failed!";

	/*if (!manager.Request("Resources/DoctorWho.wav", AudioAttribute::playOnce, 0.5))
		std::cout << "Request failed!";*/

	if (!manager.Request("Resources/Priestess.wav", AudioAttribute::playOnce, 0.5))
		std::cout << "Request failed!";

	while (true)
	{
		manager.Update();
	}
	
}

int main()
{
	std::thread thrd(ThreadB);

	/*if (!manager.Request("Resources/Rasputin.wav", AudioAttribute::playOnce, 0.5))
		std::cout << "Request failed!";*/

	if (!manager.Request("Resources/StarWars3.wav", AudioAttribute::playOnce, 0.5))
		std::cout << "Request failed!";

	Pa_Sleep(1000);

	if (!manager.Request("Resources/example.wav", AudioAttribute::playOnce, 0.5))
		std::cout << "Request failed!";

	Pa_Sleep(300000);
	
	thrd.join();
}