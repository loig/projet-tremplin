package main

import (
	"bufio"
	"flag"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"net"
	"os"
	"strconv"
	"strings"
	"sync"
)

// TODO:
// * enregistrer régulièrement
// * pouvoir démarrer du dernier enregistrement
// * récupérer stats : temps, nb mouvements

// return codes
const (
	everythingOk int = iota
	wrongLab
	unknownStudent
	receptionError
	wrongMessage
)

type student struct {
	labHash   string // hash of the lab that has to be solved
	numTried  int    // number of labs that the student tried to solve
	numSolved int    // number of labs that the student effectively solved
}

var students map[string]student
var studentsMutex sync.Mutex
var isFresh bool
var updatesLogger *log.Logger

const (
	firstLab = "0:0:0:0:0:0:0:0:0"
	seed     = 0
	logfile  = "updates.log"
)

func init() {

	// random generator
	log.Print("Initializing random generator")
	rand.Seed(seed)

	// command line reading
	log.Print("Initializing command line analysis")
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
	// initialize the students map
	initializeStudentsMapFromFile(flag.Args()[0])
	// logging of students map updates
	if isFresh {
		log.Print("Initializing new log file")
		logfileWriter, err := os.OpenFile(logfile, os.O_WRONLY|os.O_TRUNC|os.O_CREATE, 0777)
		if err != nil {
			log.Fatal("Cannot create log file (", logfile, ")")
		}
		updatesLogger = log.New(logfileWriter, "", 0)
	} else {
		// populate the students map
		populateStudentsMapFromLog()
		// open log file in append mode
		log.Print("Opening log file")
		logfileWriter, err := os.OpenFile(logfile, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0777)
		if err != nil {
			log.Fatal("Cannot open log file (", logfile, ")")
		}
		updatesLogger = log.New(logfileWriter, "", 0)
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
	defer log.Print("Ending interaction with ", conn.RemoteAddr())

	// read information sent by student
	reader := bufio.NewReader(conn)
	msg, err := reader.ReadString('\n')
	if err != nil {
		log.Print(conn.RemoteAddr(), " Error in message reception: ", err)
		sendInfo(receptionError, conn, student{})
		return
	}

	// parse information sent by student
	splitMsg := strings.Split(msg, ":::")
	if len(splitMsg) < 3 {
		log.Print(conn.RemoteAddr(), " Wrong message received: ", msg)
		sendInfo(wrongMessage, conn, student{})
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
		sendInfo(unknownStudent, conn, studentInfo)
		return
	}
	log.Print(conn.RemoteAddr(), " is ", studentID)

	// second element of the msg is lab hash
	// check that it corresponds to current
	// request to student
	labHash := splitMsg[1]
	if labHash != studentInfo.labHash {
		log.Print(conn.RemoteAddr(), " Wrong lab solved: '", labHash, "' instead of '", studentInfo.labHash, "'")
		sendInfo(wrongLab, conn, studentInfo)
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

	// get next lab
	log.Print(conn.RemoteAddr(), " had to solve ", studentInfo.labHash)
	studentInfo.labHash = getNextLab(studentInfo.labHash)
	log.Print(conn.RemoteAddr(), " must now solve ", studentInfo.labHash)

	// update the students map
	studentsMutex.Lock()
	updatesLogger.Print(studentID, ":::", studentInfo.labHash, ":::", studentInfo.numSolved, ":::", studentInfo.numTried)
	students[studentID] = studentInfo
	studentsMutex.Unlock()

	// say everything is ok to the student
	sendInfo(everythingOk, conn, studentInfo)

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

// read the log file to restore the content
// of the students map
func populateStudentsMapFromLog() {
	log.Print("Restoring students map from log file (", logfile, ")")
	defer log.Print("Students map restoration finished")

	logFileReader, err := os.Open(logfile)
	if err != nil {
		log.Fatal("Cannot read log file (", logfile, ")")
	}
	bufReader := bufio.NewReader(logFileReader)
	for line, err := bufReader.ReadString('\n'); err != io.EOF; line, err = bufReader.ReadString('\n') {
		line = strings.Trim(line, "\r\n")
		splitLine := strings.Split(line, ":::")
		if len(splitLine) != 4 {
			log.Print("Ignoring a line (wrong size): ", line)
			continue
		}
		studentID := splitLine[0]
		studentInfo, found := students[studentID]
		if !found {
			log.Print("Ignoring a line (unknown student): ", line)
			continue
		}
		nextHash := splitLine[1]
		numSolved, err := strconv.Atoi(splitLine[2])
		if err != nil {
			log.Print("Ignoring a line (numSolved cannot be read): ", line)
			continue
		}
		numTried, err := strconv.Atoi(splitLine[3])
		if err != nil {
			log.Print("Ignoring a line (numTried cannot be read): ", line)
			continue
		}
		if studentInfo.numTried < numTried {
			studentInfo.numTried = numTried
			studentInfo.numSolved = numSolved
			studentInfo.labHash = nextHash
			students[studentID] = studentInfo
		}
	}
}

// create a new student from her name
func newStudent(name string) student {
	var theStudent student
	theStudent.labHash = firstLab
	return theStudent
}

// get the next lab hash from a lab hash
func getNextLab(labHash string) string {
	upLeft := rand.Intn(32768)          // no wall on uper left position (start)
	bottomRight := 2 * rand.Intn(32768) // no wall on bottom right position (goal)
	return fmt.Sprint(
		upLeft, ":", rand.Intn(65536), ":", rand.Intn(65536), ":",
		rand.Intn(65536), ":", rand.Intn(65536), ":", rand.Intn(65536), ":",
		rand.Intn(65536), ":", rand.Intn(65536), ":", bottomRight,
	)
}

// send information to a student
func sendInfo(code int, conn net.Conn, studentInfo student) {
	writer := bufio.NewWriter(conn)
	log.Print(conn.RemoteAddr(), " Sending code ", code)
	writer.WriteString(fmt.Sprint(code, ":"))
	writer.WriteString(studentInfo.labHash)
	writer.Flush()
}
