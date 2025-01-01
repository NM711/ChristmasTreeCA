COMP="g++"


generate_dir:
	mkdir build
	mkdir ./build/depen
	mkdir ./build/bin

build:
	make generate_dir
	$(COMP) -c ./lib/ecautomata/ecautomata.cpp ./lib/ecautomata/util/seeder.cpp ./lib/ecautomata/util/grid.cpp
	ar rvs ecautomata.a ./ecautomata.o ./seeder.o ./grid.o  
	$(COMP) -o tree ./main.cpp -L . -l:ecautomata.a
	mv *.o *.a ./build/depen

clean:
	rm -rf ./build
