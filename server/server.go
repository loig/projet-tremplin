package main

import (
	"bufio"
	"flag"
	"fmt"
	"io/ioutil"
	"log"
	"net"
	"os"
	"strings"
	"sync"
)

// TODO:
// * enregistrer régulièrement
// * pouvoir démarrer du dernier enregistrement
// * définir le prochain labyrinthe à résoudre
// * récupérer stats : temps, nb mouvements
// * réponse du serveur à l'étudiant qui soumet un message

type student struct {
	labHash   string // hash of the lab that has to be solved
	numTried  int    // number of labs that the student tried to solve
	numSolved int    // number of labs that the student effectively solved
}

var students map[string]student
var studentsMutex sync.Mutex
var isFresh bool

func init() {
	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage: %s [-new|-h] studentsList\n", os.Args[0])
		flag.PrintDefaults()
	}

	flag.BoolVar(&isFresh, "new", false, "Start a fresh server. If this option is not set, the server starts from the last save.")
}

func main() {

	// read command line
	flag.Parse()
	if flag.NArg() < 1 {
		fmt.Fprintf(os.Stderr, "Error: Not enough arguments")
		flag.Usage()
		return
	}

	students = make(map[string]student)
	if isFresh {
		// initialize the students map
		initializeStudentsMapFromFile(flag.Args()[0])
	} else {
		// populate the students map
	}

	// start the server
	listener, err := net.Listen("tcp", ":8192")
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print("Wrong connexion: ", err)
			continue
		}
		go handleConnexion(conn)
	}

}

// handle a request from a student
func handleConnexion(conn net.Conn) {
	log.Print("Starting interaction with ", conn.RemoteAddr())
	defer log.Print("Ending interaction with", conn.RemoteAddr())

	// read information sent by student
	reader := bufio.NewReader(conn)
	msg, err := reader.ReadString('\n')
	if err != nil {
		log.Print(conn.RemoteAddr(), " Error in message reception: ", err)
		return
	}

	// parse information sent by student
	splitMsg := strings.Split(msg, ":::")
	if len(splitMsg) < 3 {
		log.Print(conn.RemoteAddr(), " Wrong message received: ", msg)
		return
	}

	// first element of the msg is student id
	// check that it exists
	studentID := splitMsg[0]
	studentsMutex.Lock()
	studentInfo, found := students[studentID]
	studentsMutex.Unlock()
	if !found {
		log.Print(conn.RemoteAddr(), " This student does not exist: ", studentID)
		return
	}
	log.Print(conn.RemoteAddr(), " is ", studentID)

	// second element of the msg is lab hash
	// check that it corresponds to current
	// request to student
	labHash := splitMsg[1]
	if labHash != studentInfo.labHash {
		log.Print(conn.RemoteAddr(), " Wrong lab solved: ", labHash, " instead of ", studentInfo.labHash)
		return
	}
	// the student tried one more lab
	studentInfo.numTried++
	log.Print(conn.RemoteAddr(), " Number of tries by ", studentID, " is now ", studentInfo.numTried)

	// third element of the msg is a flag telling
	// if the lab was solved correctly (i.e. arrival
	// is reached if possible, and else it is stated
	// that the arrival cannot be reached)
	resultFlag := strings.Trim(splitMsg[2], "\r\n")
	if resultFlag == "ok" {
		studentInfo.numSolved++
		log.Print(conn.RemoteAddr(), " Lab solved!")
	} else {
		log.Print(conn.RemoteAddr(), " Lab not solved, flag is ", resultFlag)
	}
	log.Print(conn.RemoteAddr(), " Number of solved by ", studentID, " is now ", studentInfo.numSolved)

	// update the students map
	studentsMutex.Lock()
	students[studentID] = studentInfo
	studentsMutex.Unlock()

	return
}

// read a file with student names and create
// the students map from it
func initializeStudentsMapFromFile(fileName string) {
	log.Print("Starting students map initialization")
	log.Print("Reading file ", fileName)
	studentsList, err := ioutil.ReadFile(fileName)
	if err != nil {
		log.Fatal("Problem while reading students file ", fileName, ": ", err)
	}
	studentsNames := strings.Split(string(studentsList), "\n")
	for _, name := range studentsNames {
		if len(name) > 0 {
			log.Print("Adding ", name, " to the students map")
			students[name] = newStudent(name)
		}
	}
	log.Print("Ending students map initialization")
}

// creat a new student from her name
func newStudent(name string) student {
	var theStudent student
	theStudent.labHash = "test"
	return theStudent
}
