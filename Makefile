fiber:main.cpp common.cpp
	g++ -rdynamic -g main.cpp common.cpp fiber.cpp log.cpp zyx_thread.cpp scheduler.cpp -I ./include -lpthread -o scheduler 