from tkinter import *
import tkinter as tk 
from tkinter import ttk

root = tk.Tk()

canvas = tk.Canvas(root, width=400, height=400, borderwidth=0, bg="black")
canvas.grid()

SelectedNode = 0

# if a node has already been selected it creates an edge
def selNodes(n):
    global SelectedNode

    if(SelectedNode != n and SelectedNode != 0):
        print("MULTISELECT")
        e = Edge(SelectedNode, n)
        
    else:
        SelectedNode = n

edges = []

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

        edges.append(self)

    def update(self):
        # the points of the line
        points = [self.source.pos[0], self.source.pos[1], self.target.pos[0], self.target.pos[1]]
        
        # updating the start and end points of the line
        self.canvas.coords(self.line, points)

class Node:
    def __init__(self, canvas, x, y, r):
        # creating the shape

        # basically all of the rubish needed for it to work
        x1 = x - r
        y1 = y - r
        x2 = x + r
        y2 = y + r

        self.shape = canvas.create_oval(x1, y1, x2, y2, fill="red")
        self.canvas = canvas

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
        self.name = "Jeff " + str(x)

        # getting the new centre
        self.centre = self.get_circle_center()

        # adding the text to the node
        self.text = canvas.create_text(self.centre[0], self.centre[1], text=self.name, state=tk.DISABLED, justify="center")
    
    def press(self, event):
        global SelectedNode

        # selects the new node
        selNodes(self)
        canvas.itemconfig(self.shape, fill="blue")

    def drag(self, event):
        # offset it because it's radius is 50
        dx = (event.x-50)
        dy = (event.y-50)

        # moving the shape
        canvas.moveto(self.shape, dx, dy)

        # moving the text
        self.get_circle_center()
        canvas.moveto(self.text, self.centre[0]-15, self.centre[1]-5)

    # updates the position for the edge
    def moveend(self, event):
        global edges

        self.pos = self.get_circle_center()

        for e in edges:
            e.update()


    def get_circle_center(self):
        x1, y1, x2, y2 = self.canvas.coords(self.shape)
        center_x = (x1 + x2) / 2
        center_y = (y1 + y2) / 2

        self.centre = (center_x, center_y)
        return center_x, center_y

def

s1 = Node(canvas, 20, 20, 50)
s2 = Node(canvas, 0, 0, 50)



root.mainloop()