"""
A program that stores the following information about each book:

Title, Author
Year, ISBN

User can:

View all records
Search an entry
Add entry
Updatre entry
Delete
Clsoe
"""

from tkinter import *

import Backend

def getSelectedRow(event):
    global selectedTupple
    index = listBox.curselection()[0]
    selectedTupple = listBox.get(index)

    entry1.delete(0, END)
    entry1.insert(END, selectedTupple[1])
    entry2.delete(0, END)
    entry2.insert(END, selectedTupple[2])
    entry3.delete(0, END)
    entry3.insert(END, selectedTupple[3])
    entry4.delete(0, END)
    entry4.insert(END, selectedTupple[4])            

def viewCommand():
    listBox.delete(0, END)

    for row in Backend.view():
        # Adds the new tuple at the end of existing ones
        listBox.insert(END, row)

def searchCommand():
    listBox.delete(0, END)

    for row in Backend.search(titleText.get(), authorText.get(), yearText.get(), isbnText.get()):
        listBox.insert(END, row)

def addCommand():
    Backend.insert(titleText.get(), authorText.get(), yearText.get(), isbnText.get())
    listBox.delete(0, END)
    listBox.insert(END, (titleText.get(), authorText.get(), yearText.get(), isbnText.get()))

def deleteCommand():
    Backend.delete(selectedTupple[0])
    viewCommand()

def updateCommand():
    Backend.update(selectedTupple[0], titleText.get(), authorText.get(), yearText.get(), isbnText.get())

window = Tk()
window.wm_title("Book Store")

title = Label(window, text="Title")
title.grid(row=0, column=0)

author = Label(window, text="Author")
author.grid(row=0, column=2)

year = Label(window, text="Year")
year.grid(row=1, column=0)

isbn = Label(window, text="ISBN")
isbn.grid(row=1, column=2)

titleText = StringVar()
entry1 = Entry(window, textvariable=titleText)
entry1.grid(row=0, column=1)

authorText = StringVar()
entry2 = Entry(window, textvariable=authorText)
entry2.grid(row=0, column=3)

yearText = StringVar()
entry3 = Entry(window, textvariable=yearText)
entry3.grid(row=1, column=1)

isbnText = StringVar()
entry4 = Entry(window, textvariable=isbnText)
entry4.grid(row=1, column=3)

listBox = Listbox(window, height=6, width=35)
listBox.grid(row=2, column=0, rowspan=6, columnspan=2)

scrollbar = Scrollbar(window)
scrollbar.grid(row=2, column=2, rowspan=6)

listBox.configure(yscrollcommand=scrollbar.set)
scrollbar.configure(command=listBox.yview)

listBox.bind('<<ListboxSelect>>', getSelectedRow)

viewAll = Button(window, text="View all", width=12, command=viewCommand)
viewAll.grid(row=2, column=3)

serachEntry = Button(window, text="Search entry", width=12, command=searchCommand)
serachEntry.grid(row=3, column=3)

addEntry = Button(window, text="Add entry", width=12, command=addCommand)
addEntry.grid(row=4, column=3)

Update = Button(window, text="Update", width=12, command=updateCommand)
Update.grid(row=5, column=3)

delete = Button(window, text="Delete", width=12, command=deleteCommand)
delete.grid(row=6, column=3)

close = Button(window, text="Close", width=12, command=window.destroy)
close.grid(row=7, column=3)

window.mainloop()