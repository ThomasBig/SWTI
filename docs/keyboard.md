# Keyboard

Keyboard is a object that checks keyboard keys of keyboard or mouse. It has 5 methods.

## Examples
Following examples show basic usage of the keyboard object.
```c++
if (Keyboard.getPressed(VK_UP)) // UP arrow was pressed
while (Keyboard.get('A')) // While 'A' key is down
```

## Full documentation

### bool Keyboard.get(int key)
This function gets current keyboard input

### bool Keyboard.getPressed(int key)
This function gets current keyboard input.

### bool Keyboard.getReleased(int key)
This function gets current keyboard input

### bool Keyboard.wait(unsigned int ticks)
This function waits specified number of ticks.

### bool Keyboard.waitUser()
This function stops the application until user presses any button.
Releasing a button doesn't trigger this function.
