all:
	@find * -name "*.java" > sources.txt
	@javac @sources.txt

run: all
	@java com.enesates.avaj.simulator.Simulator scenario.txt

c: clean
clean:
	@rm -f sources.txt
	@rm -f simulation.txt
	@rm -f $$(find * -name "*.class")
