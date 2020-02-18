all: MyBot.java
	javac ./hlt/*.java
	javac ./myBot/*.java
	javac MyBot.java
	javac ./org/la4j/*.java

run:
	java MyBot

clean:
	rm -rf *.class ./hlt/*.class ./myBot/*.class errorlog* 
	find ./org -type f -name "*.class" -delete