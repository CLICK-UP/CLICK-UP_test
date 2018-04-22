package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"os/exec"
	"strconv"
)

var Usage = func() {
	fmt.Println("USAGE: CLICK-UP-TEST command [arguments] ...")
	fmt.Println("The commands are:\n\t-scp\t\t#use scp to transport data")
	fmt.Println("\t\t--limit\t#limit the triffic speed")
	fmt.Println("\t\t--source\t#source file path")
	fmt.Println("\t\t--destination\t#target folder path")
}

func main() {
	args := os.Args
	if args == nil || len(args) <= 6 {
		Usage()
		return
	}

	switch args[0] {
	case "scp":
		if len(args) != 7 {
			fmt.Println("USAGE : CLICK-UP-TEST scp --limit <x> --source <y> --destination <z>")
			return
		}
		_, errLimit := strconv.Atoi(args[2])
		src := args[4]
		dest := args[6]
		if errLimit != nil {
			fmt.Println("USAGE : CLICK-UP-TEST scp --limit <x> --source <y> --destination <z>")
			return
		}
		cmd := exec.Command(args[0], args[2], src, dest)
		stdout, err := cmd.StdoutPipe()
		if err != nil {
			log.Fatal(err)
		}
		defer stdout.Close()
		if err := cmd.Start(); err != nil {
			log.Fatal(err)
		}
		opBytes, err := ioutil.ReadAll(stdout)
		if err != nil {
			log.Fatal(err)
		}
		log.Println(string(opBytes))
	}
}
