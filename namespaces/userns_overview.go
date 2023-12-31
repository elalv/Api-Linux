/* userns_overview.go

   Display a hierarchical view of the user namespaces on the system along with
   the member processes for each namespace. This requires features new in
   Linux 4.9. See the ioctl_ns(2) man page.

   For an expanded version of this program, see namespaces_of.go.
*/

package main

import (
	"fmt"
	"io/ioutil"
	"os"
	"sort"
	"strconv"
	"strings"
	"syscall"
	"unsafe"
)

// A namespace is identified by device ID and inode number.

type namespaceID struct {
	device    uint64 // dev_t
	inode_num uint64 // ino_t
}

// A namespace has associated attributes: a set of child namespaces and a set
// of member processes.

type namespaceAttribs struct {
	children []namespaceID // Child namespaces
	pids     []int         // Member processes
}

// The following map records all of the namespaces that we find on the system.

var nsList = make(map[namespaceID]*namespaceAttribs)

// Along the way, we'll discover the ancestor of all user namespaces (the root
// of the user namespace hierarchy).

var initialNS namespaceID

// addNamespace() adds a PID to the list of PIDs associated with the user
// namespace referred to by 'namespaceFD'.
//
// The set of namespaces is recorded in the 'nsList' map. If the map does not
// yet contain an entry corresponding to 'namespaceFD', then an entry is
// created. This process is recursive: if the parent of the user namespace
// referred to by 'namespaceFD' does not have an entry in 'nsList' then an
// entry is created for the parent, and the namespace referred to by
// 'namespaceFD' is made a child of that namespace.
//
// When called recursively to create the ancestor namespace entries, this
// function is called with 'pid' as -1, meaning that no PID needs to be added
// for this namespace entry.
//
// The return value of the function is the ID of the namespace entry (i.e., the
// device ID and inode number corresponding to the user namespace file referred
// to by 'namespaceFD').

func addNamespace(namespaceFD int, pid int) namespaceID {
	const NS_GET_PARENT = 0xb702 // ioctl() to get namespace parent
	var sb syscall.Stat_t

	// Obtain the device ID and inode number of the namespace file. These
	// values together form the key for the 'nsList' map entry.

	err := syscall.Fstat(namespaceFD, &sb)
	if err != nil {
		fmt.Println("syscall.Fstat():", err)
		os.Exit(1)
	}

	nsid := namespaceID{sb.Dev, sb.Ino}

	if _, fnd := nsList[nsid]; fnd {

		// Namespace already exists; nothing to do.

	} else {

		// Namespace entry does not yet exist; create it.

		nsList[nsid] = new(namespaceAttribs)

		// Get file descriptor for parent user namespace.

		r, _, err := syscall.Syscall(syscall.SYS_IOCTL,
			uintptr(namespaceFD), uintptr(NS_GET_PARENT), 0)
		parentFD := (int)((uintptr)(unsafe.Pointer(r)))

		if parentFD == -1 {
			switch err {
			case syscall.EPERM:
				// This is the initial NS; remember it.
				initialNS = nsid
			case syscall.ENOTTY:
				fmt.Println("This kernel doesn't support " +
					"namespace ioctl() operations")
				os.Exit(1)
			default:
				// Unexpected error; bail.
				fmt.Println("ioctl():", err)
				os.Exit(1)
			}
		} else {

			// We have a parent user namespace; make sure it has an
			// entry in the map. No need to add any PID for the
			// parent entry.

			p := addNamespace(parentFD, -1)

			// Make the current namespace entry ('nsid') a child of
			// the parent namespace entry.

			nsList[p].children = append(nsList[p].children, nsid)

			syscall.Close(parentFD)
		}
	}

	// Add PID to PID list for this namespace entry.

	if pid > 0 {
		nsList[nsid].pids = append(nsList[nsid].pids, pid)
	}

	return nsid
}

// processProcFile() processes a single /proc/PID entry, creating a namespace
// entry for this PID's /proc/PID/ns/user file (and, as necessary, namespace
// entries for all ancestor namespaces going back to the initial user
// namespace). 'name' is the name of a PID directory under /proc.

func processProcFile(name string) {

	// Obtain a file descriptor that refers to the user namespace
	// of this process.

	namespaceFD, err := syscall.Open("/proc/"+name+"/ns/user",
		syscall.O_RDONLY, 0)

	if namespaceFD < 0 {
		fmt.Println("open():", err)
		os.Exit(1)
	}

	pid, _ := strconv.Atoi(name)

	addNamespace(namespaceFD, pid)

	syscall.Close(namespaceFD)
}

// DisplayNamespaceTree() recursively displays the namespace tree rooted at
// 'nsid'. 'level' is our current level in the tree, and is used for producing
// suitably indented output.

func displayNamespaceTree(nsid namespaceID, level int) {

	indent := strings.Repeat(" ", level*4)

	// Display the namespace ID (device ID + inode number).

	fmt.Print(indent)
	fmt.Println(nsid)

	// Print a sorted list of the PIDs that are members of this namespace.
	// We do a bit of a dance here to produce a list of PIDs that is
	// suitably wrapped and indented, rather than a long single-line list.

	sort.Ints(nsList[nsid].pids)
	base := len(indent) + 25
	col := base
	for i, p := range nsList[nsid].pids {
		if i == 0 || col >= 80 && col > base+32 {
			col = base
			if i > 0 {
				fmt.Println()
			}
			fmt.Print(indent)
			fmt.Print("            ")
			if i == 0 {
				fmt.Print("PIDs: ")
			} else {
				fmt.Print("      ")
			}
		}
		fmt.Print(strconv.Itoa(p) + " ")
		col += len(strconv.Itoa(p)) + 1
	}
	fmt.Println()

	// Recursively display the child namespaces.

	for _, v := range nsList[nsid].children {
		displayNamespaceTree(v, level+1)
	}
}

func main() {

	// Fetch a list of the filenames under /proc.

	files, err := ioutil.ReadDir("/proc")
	if err != nil {
		fmt.Println("ioutil.Readdir():", err)
		os.Exit(1)
	}

	// Process each /proc/PID (PID starts with a digit).

	for _, f := range files {
		if f.Name()[0] >= '1' && f.Name()[0] <= '9' {
			processProcFile(f.Name())
		}
	}

	// Display the namespace tree rooted at the initial user namespace.

	displayNamespaceTree(initialNS, 0)
}