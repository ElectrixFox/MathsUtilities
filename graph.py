import tkinter as tk 
from tkinter import ttk
from tkinter import font
import math
import re

curindex = 1
numbers = []
factors = []

def loadData(filePath):
    global numbers, factors

    file = open(filePath, 'r')

    pattern = r"(\d+):\s*((?:\d+\s*\*\s*)*\d+)$"
    lines = file.readlines()

    for s in lines:
        match = re.match(pattern, s)
        if match:
            number = int(match.group(1))
            factor_strings = match.group(2).split('*')
            facs = [int(factor.strip()) for factor in factor_strings]

            numbers.append(number)
            factors.append(facs)

loadData("Composite.txt")

root = tk.Tk()

canvas = tk.Canvas(root, width=400, height=400, borderwidth=0, bg="black")
canvas.grid(row = 0, column = 0, sticky="nsew")

SelectedNode = 0

class controlBar():
    def __init__(self, root, canvas):
        self.root = root
        self.root.title("Sidebar with Canvas Example")

        # Create main frame with grid layout
        self.main_frame = ttk.Frame(root)
        self.main_frame.grid(row=0, column=1, sticky="nsew")

        # Create Canvas and add it to the main frame
        self.canvas = canvas
        
        # Create sidebar frame and add it to the main frame
        self.sidebar_frame = ttk.Frame(self.main_frame)
        self.sidebar_frame.grid(row=0, column=1, sticky="ns")

        # Add widgets to the sidebar
        button1 = ttk.Button(self.sidebar_frame, text="Next Number")
        button1.pack(pady=10)

        self.nextbutton = button1

        # text input box
        text1 = ttk.Entry(self.sidebar_frame)
        text1.pack(pady = 10)
        self.textbox = text1

        #button2 = ttk.Button(self.sidebar_frame, text="Option 2")
        #button2.pack(pady=10)

        #button3 = ttk.Button(self.sidebar_frame, text="Option 3")
        #button3.pack(pady=10)

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

edges = []


cb = controlBar(root, canvas)

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
        points = [self.source.pos[0], self.source.pos[1], self.target.pos[0], self.target.pos[1]]
        
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

nodes = []

def load_new_set():
    global numbers, factors, curindex

    n = Node(canvas, 20, 20, 20, str(numbers[curindex]), "blue")
    nodes.append(n)

    facs = factors[curindex]

    for i in range(1, len(facs)+1):
        if(facs[i] == facs[i-1]):
            i += 1
            continue

        xy = 20 + 40*(i*2)
        fn = Node(canvas, xy, 20, 20, str(facs[i-1]), "red")
        nodes.append(fn)

            
    curindex += 1

cb.nextbutton.config(command=load_new_set)


#s1 = Node(canvas, 20, 20, 50, "Jeff")
#s2 = Node(canvas, 0, 0, 50, "Bob")

root.mainloop()
cb.root.mainloop()