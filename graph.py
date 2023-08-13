import tkinter as tk 
from tkinter import ttk
from tkinter import font
import math
import re

curindex = 0
numbers = []
factors = []
exponents = []

def lineEquation(c1, c2):
    # giving all of these values easy to use alias'
    x1 = c1.pos[0]
    y1 = c1.pos[1]
    r1 = c1.radius

    x2 = c2.pos[0]
    y2 = c2.pos[1]
    r2 = c2.radius

    # getting the gradient of the line between them
    m = (y2 - y1)/(x2 - x1)

    # if c2 is on the other side to c1 then add 180 to make it go full 2 pi radians (360 deg)
    O = (math.pi)/2 if (x2 < x1) else 0 

    # getting the angle between the centre of the circle and where the line passes through it's circumference
    p = math.atan(m)+O

    # cos of the angle between the line and the centre
    # where r1 is the hypotenuse of the line we find the horizontal distance and then offset it by x1 to make it local to the circle
    # same sort of thing with sin apart from it's used to find the vertical distance
    pos1 = (x1 + r1*math.cos(p), y1 + r1*math.sin(p))

    # negative here because it is intersecting a different circle at a different point
    pos2 = (x2 - r2*math.cos(p), y2 - r2*math.sin(p))

    # set out above as coordinates to make it easier to read
    return [pos1[0], pos1[1], pos2[0], pos2[1]]





def loadData(filePath):
    global numbers, factors, exponents

    # regex pattern
    pattern = r"(\d+):\s*((?:\d+\^\d+\s*\*\s*)*\d+\^\d+)$"

    # opens the file to read
    file = open(filePath, 'r')

    # reads back all of the lines
    lines = file.readlines()

    # closes file
    file.close()

    for s in lines:
        # arrays
        facs = []
        exps = []


        #  gets all of the matches of the pattern
        match = re.match(pattern, s)
        if match:
            # group 1 matches are the numbers
            number = int(match.group(1))

            # splits at the multiplication sign
            factor_strings = match.group(2).split('*')

            # splits the factor part into the exponents and the factors
            for factor_string in factor_strings:
                factor, exponent = factor_string.strip().split('^')
                facs.append(int(factor))
                exps.append(int(exponent))
            
            # adds all of them to the respective global variables
            numbers.append(number)
            factors.append(facs)
            exponents.append(exps)


loadData("Composite.txt")

root = tk.Tk()

# creates the canvas
canvas = tk.Canvas(root, width=800, height=800, borderwidth=0, bg="black")
canvas.grid(row = 0, column = 0, sticky="nsew")

SelectedNode = 0

edges = []

nodes = []

pnodes = []

print(numbers[0])
print(factors[0])

class controlBar():
    def __init__(self, root, canvas):
        self.root = root
        self.root.title("Sidebar with Canvas Example")

        # create main frame with grid layout
        self.main_frame = ttk.Frame(root)
        self.main_frame.grid(row=0, column=1, sticky="nsew")

        # create Canvas and add it to the main frame
        self.canvas = canvas
        
        # create sidebar frame and add it to the main frame
        self.sidebar_frame = ttk.Frame(self.main_frame)
        self.sidebar_frame.grid(row=0, column=1, sticky="ns")

        # add widgets to the sidebar
        button1 = ttk.Button(self.sidebar_frame, text="Next Number")
        button1.pack(pady=10)

        self.nextbutton = button1

        # text input box
        text1 = ttk.Entry(self.sidebar_frame)
        text1.pack(pady = 10)
        self.textbox = text1

        # information on the factors
        self.factable = ttk.Treeview(self.sidebar_frame, columns=("Number", "Factors", "Exponents"))
        self.factable.heading("#0", text="", anchor="w")
        self.factable.heading("Number", text="Number")
        self.factable.heading("Factors", text="Factors")
        self.factable.heading("Exponents", text="Exponents")

        self.factable.column("#0", width=1, stretch=False)
        self.factable.column("Number", width=50)
        self.factable.column("Factors", width=100)
        self.factable.column("Exponents", width=100)

        self.factable.pack(pady=10)

cb = controlBar(root, canvas)

# if a node has already been selected it creates an edge
def selNodes(n):
    global SelectedNode

    if(SelectedNode != n and SelectedNode != 0):
        print("MULTISELECT")
        e = Edge(SelectedNode, n)
        canvas.itemconfig(SelectedNode.shape, fill=SelectedNode.colour)
        canvas.itemconfig(n.shape, fill=n.colour)
        
        SelectedNode = 0
    elif(SelectedNode == n):
        canvas.itemconfig(n.shape, fill=n.colour)
        SelectedNode = 0
    else:
        canvas.itemconfig(n.shape, fill="grey")
        SelectedNode = n

class Edge:
    def __init__(self, source, target):
        global edges

        # all of the variables getting set
        self.source = source
        self.target = target

        pos1 = source.pos
        pos2 = target.pos

        self.canvas = source.canvas

        # debug positions
        print("(" + str(pos1[0]) + ", " + str(pos1[1]) + ")")
        print("(" + str(pos2[0]) + ", " + str(pos2[1]) + ")")

        # line creation
        self.line = canvas.create_line(pos1[0], pos1[1], pos2[0], pos2[1], fill="white")
        canvas.tag_bind(self.line, '<Button-1>', self.instext)
        canvas.tag_bind(self.line, '<Button-3>', self.rem)

        self.text = self.canvas.create_text(0, 0, text = "", fill="white")
        canvas.tag_bind(self.text, '<Button-3>', self.rem)

        edges.append(self)

    def update(self):
        # the points of the line
        #points = [self.source.pos[0], self.source.pos[1], self.target.pos[0], self.target.pos[1]]
        points = lineEquation(self.source, self.target)

        # updating the start and end points of the line
        self.canvas.coords(self.line, points)

        midx = (self.source.pos[0] + self.target.pos[0])/2
        midy = (self.source.pos[1] + self.target.pos[1])/2
        
        self.canvas.coords(self.text, midx, midy)

    
    def rem(self, event):
        self.canvas.delete(self.text)
        self.canvas.delete(self.line)

    
    def instext(self, event):
        midx = (self.source.pos[0] + self.target.pos[0])/2
        midy = (self.source.pos[1] + self.target.pos[1])/2

        ltext = cb.textbox.get()

        if(ltext == ""):
            cb.textbox.bell()
        else:
            print("Creating text")
            self.canvas.itemconfig(self.text, text = ltext)
            self.canvas.coords(self.text, midx, midy)

def circle(canvas, x, y, r):
    x1 = x - r
    y1 = y - r
    x2 = x + r
    y2 = y + r

    return canvas.create_oval(x1, y1, x2, y2, fill="white")

class Node:
    def __init__(self, canvas, x, y, r, name="jeff", colour="red"):
        # basically all of the rubish needed for the circle to draw using the (x, y) and radius
        x1 = x - r
        y1 = y - r
        x2 = x + r
        y2 = y + r

        # creating the shape
        self.shape = canvas.create_oval(x1, y1, x2, y2, fill=colour)
        self.canvas = canvas

        # setting the colour
        self.colour = colour

        # setting the radius
        self.radius = r

        # binding the events
        canvas.tag_bind(self.shape, '<Double-Button-1>', self.press)
        canvas.tag_bind(self.shape, '<B1-Motion>', self.drag)
        canvas.tag_bind(self.shape, '<ButtonRelease-1>', self.moveend)

        # setting the position up
        self.pos = (x, y)

        print("(" + str(self.pos[0]) + ", " + str(self.pos[1]) + ")")

        # identifier name
        self.name = name

        # text font
        self.font = font.Font(family="Arial", size=12)

        # getting the new centre
        self.centre = self.get_circle_center()

        # adding the text to the node
        self.text = canvas.create_text(self.centre[0], self.centre[1], text=self.name, state=tk.DISABLED, justify="center", anchor='center', font=self.font)
    
    def press(self, event):
        # selects the new node
        selNodes(self)

    def drag(self, event):
        # offset it because it's radius is 50
        dx = (event.x-self.radius)
        dy = (event.y-self.radius)

        # moving the shape
        canvas.moveto(self.shape, dx, dy)

        # gets the height of the font
        fheight = self.font.metrics("linespace")

        # getting the offset of the text to center it
        offset = math.floor(fheight/2)

        # moving the text
        self.get_circle_center()
        canvas.moveto(self.text, self.centre[0]-offset, self.centre[1]-offset)

    # updates the position for the edge
    def moveend(self, event):
        global edges

        self.pos = self.get_circle_center()

        for e in edges:
            e.update()

    # does what it says on the tin
    def get_circle_center(self):
        x1, y1, x2, y2 = self.canvas.coords(self.shape)
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2

        self.centre = (center_x, center_y)
        return center_x, center_y

def load_new_set():
    global numbers, factors, exponents, curindex

    # creates the new node
    n = Node(canvas, 20, 20, 20, str(numbers[curindex]), "green")
    nodes.append(n)

    # gets the factors of the number
    facs = factors[curindex]
    cb.factable.insert("", "end", values=(numbers[curindex], factors[curindex], exponents[curindex]))

    print(factors[curindex])
    curindex += 1

    for i in range(1, len(facs)+1):
        # the factor as a string
        name = str(facs[i-1])

        # goes to the next factor if the name is already there
        if(name in pnodes):
            continue
        
        # calculation to get the spacing of 40 units between each node
        xy = 20 + 40*(i*2)

        # creates the new factor node
        fn = Node(canvas, xy, 20, 20, name, "red")

        # appending the name to the prime list
        pnodes.append(name)

        # appending the node to the nodes list
        nodes.append(fn)
      


cb.nextbutton.config(command=load_new_set)


#s1 = Node(canvas, 20, 20, 50, "Jeff")
#s2 = Node(canvas, 0, 0, 50, "Bob")

root.mainloop()
cb.root.mainloop()