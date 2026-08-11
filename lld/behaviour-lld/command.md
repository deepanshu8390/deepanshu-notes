# Command Design Pattern

## Content

> **Core learning flow:** Problem Statement → My Design → Problem → Command Intuition → Roles → C++ → Java → Client Flow → Key Points

---

# 1. Problem Statement

Imagine a TV remote system.

TV ke paas actual operations hain:

```text
TV
├── turnOn()
├── turnOff()
├── changeChannel()
└── adjustVolume()
```

Remote ke paas buttons hain:

```text
Remote
├── ON
├── OFF
├── Channel
└── Volume
```

Requirement:

> User remote ka button press kare aur corresponding TV action perform ho.

Future mein:

- new actions add ho sakti hain
- different devices aa sakte hain, e.g. AC, speaker
- commands ko store/queue karke later execute karna ho sakta hai
- same request ko later execute karna ho sakta hai

---

# 2. My Initial Design — Traditional Way

Remote directly TV ko call kare:

```text
User
  ↓
Remote
  ↓
TV
```

C++ idea:

```cpp
class RemoteControl {
private:
    TV* tv;

public:
    RemoteControl(TV* tv) {
        this->tv = tv;
    }

    void pressOnButton() {
        tv->turnOn();
    }

    void pressOffButton() {
        tv->turnOff();
    }

    void pressChannelButton(int channel) {
        tv->changeChannel(channel);
    }

    void pressVolumeButton(int volume) {
        tv->adjustVolume(volume);
    }
};
```

## Is design ka thought

Simple case mein ye bilkul understandable hai:

```text
Remote → direct TV method call
```

Problem tab start hoti hai jab remote mein functionality continuously badhne lage.

---

# 3. Problem in Traditional Design

## Problem 1 — Remote keeps knowing every action

New action aaya:

```text
Netflix
Mute
YouTube
Cinema Mode
```

Toh Remote mein naye methods/additional logic aate jayenge:

```text
pressNetflixButton()
pressMuteButton()
pressYouTubeButton()
...
```

Remote धीरे-धीरे har possible action jaane lagta hai.

## Problem 2 — Different devices

Kal TV ke saath:

```text
AC
Speaker
Lights
Projector
```

bhi control karne hain.

Remote ko har device ke concrete methods aur operations ka knowledge hone lagega.

## Problem 3 — Request ko store/later execute karna difficult

Suppose user bolta hai:

```text
TV ON
Channel 5
Volume 20
```

Abhi traditional approach mein calls immediately execute hoti hain:

```cpp
tv.turnOn();
tv.changeChannel(5);
tv.adjustVolume(20);
```

But agar request ko pehle store karna ho aur baad mein execute karna ho, toh direct method calls convenient nahi hain.

---

# 4. The Turning Point — Request ko Object Bana Do

Yahi Command Pattern ka main intuition hai.

Normal approach:

```text
"TV ON karo"
    ↓
tv.turnOn()
    ↓
Immediately execute
```

Command approach:

```text
"TV ON karo"
    ↓
TurnOnCommand object
    ↓
store / pass / queue / execute later
```

So:

> **Request/action ko direct execute karne ke bajaye us request ko object bana do.**

Example:

```text
TurnOnCommand
TurnOffCommand
ChangeChannelCommand
AdjustVolumeCommand
```

Har command ek specific request ko represent karega.

---

# 5. Command Interface

Har command ka common interface:

```cpp
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};
```

Baby meaning:

> Har command ke paas ek common operation hoga: `execute()`.

Remote ko specific action ka logic nahi pata.

Remote simply bolega:

```text
command.execute()
```

---

# 6. Concrete Command — Example

`TurnOnCommand` actual request ko encapsulate karta hai:

```cpp
class TurnOnCommand : public Command {
private:
    TV* tv;

public:
    TurnOnCommand(TV* tv) {
        this->tv = tv;
    }

    void execute() override {
        tv->turnOn();
    }
};
```

Object ke andar:

```text
TurnOnCommand
      |
      └── TV reference
```

`execute()` call hone par:

```text
TurnOnCommand.execute()
        ↓
TV.turnOn()
        ↓
TV ON
```

---

# 7. Parameter bhi Command Object mein Store ho sakta hai

Example: channel change.

```cpp
class ChangeChannelCommand : public Command {
private:
    TV* tv;
    int channel;

public:
    ChangeChannelCommand(TV* tv, int channel) {
        this->tv = tv;
        this->channel = channel;
    }

    void execute() override {
        tv->changeChannel(channel);
    }
};
```

Suppose:

```cpp
Command* command = new ChangeChannelCommand(&tv, 5);
```

Command object ke andar:

```text
ChangeChannelCommand
├── TV reference
└── channel = 5
```

Later:

```cpp
command->execute();
```

means:

```text
TV.changeChannel(5)
```

So request + its parameters bhi object mein encapsulate ho gaye.

---

# 8. Main Roles

## 1. Command

Common interface:

```text
execute()
```

## 2. Concrete Command

Specific request ko represent karta hai.

```text
TurnOnCommand
TurnOffCommand
ChangeChannelCommand
AdjustVolumeCommand
```

## 3. Receiver

Actual kaam karta hai.

Yahan:

```text
TV = Receiver
```

TV ko pata hai:

```text
turnOn()
turnOff()
changeChannel()
adjustVolume()
```

## 4. Invoker

Command ko invoke karta hai.

Yahan:

```text
RemoteControl = Invoker
```

Remote ka kaam:

```text
command.execute()
```

Usko command ke andar ka actual logic nahi pata.

## 5. Client

Objects banata hai aur unko wire/configure karta hai.

Usually:

```text
Client
  ↓
creates Receiver
  ↓
creates Commands
  ↓
assigns Commands to Invoker
```

---

# 9. Better Remote Design — Generic Command Slots

Article mein agar Remote ke andar:

```cpp
setOnCommand()
setOffCommand()
setNetflixCommand()
...
```

banate jaoge, toh Remote khud rigid ho sakta hai.

Better:

```cpp
class RemoteControl {
private:
    vector<Command*> commands;

public:
    RemoteControl(int numberOfButtons) {
        commands.resize(numberOfButtons, nullptr);
    }

    void setCommand(int button, Command* command) {
        commands[button] = command;
    }

    void pressButton(int button) {
        if (commands[button] != nullptr) {
            commands[button]->execute();
        }
    }
};
```

Ab Remote ko ye nahi pata:

```text
TV?
Netflix?
AC?
Speaker?
```

Remote only knows:

```text
button → Command
press → execute()
```

New feature ke liye new Command object create karke kisi slot par assign kar sakte ho.

---

# 10. Complete C++ Code

```cpp
#include <iostream>
#include <vector>
using namespace std;

// ======================================================
// COMMAND INTERFACE
// ======================================================

class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};


// ======================================================
// RECEIVER
// ======================================================

class TV {
public:
    void turnOn() {
        cout << "TV is ON" << endl;
    }

    void turnOff() {
        cout << "TV is OFF" << endl;
    }

    void changeChannel(int channel) {
        cout << "Channel changed to " << channel << endl;
    }

    void adjustVolume(int volume) {
        cout << "Volume set to " << volume << endl;
    }
};


// ======================================================
// CONCRETE COMMANDS
// ======================================================

class TurnOnCommand : public Command {
private:
    TV* tv;

public:
    TurnOnCommand(TV* tv) {
        this->tv = tv;
    }

    void execute() override {
        tv->turnOn();
    }
};


class TurnOffCommand : public Command {
private:
    TV* tv;

public:
    TurnOffCommand(TV* tv) {
        this->tv = tv;
    }

    void execute() override {
        tv->turnOff();
    }
};


class ChangeChannelCommand : public Command {
private:
    TV* tv;
    int channel;

public:
    ChangeChannelCommand(TV* tv, int channel) {
        this->tv = tv;
        this->channel = channel;
    }

    void execute() override {
        tv->changeChannel(channel);
    }
};


class AdjustVolumeCommand : public Command {
private:
    TV* tv;
    int volume;

public:
    AdjustVolumeCommand(TV* tv, int volume) {
        this->tv = tv;
        this->volume = volume;
    }

    void execute() override {
        tv->adjustVolume(volume);
    }
};


// ======================================================
// INVOKER
// ======================================================

class RemoteControl {
private:
    vector<Command*> commands;

public:
    RemoteControl(int numberOfButtons) {
        commands.resize(numberOfButtons, nullptr);
    }

    // Put any command on any button.
    // Remote does not care what the command actually does.
    void setCommand(int button, Command* command) {
        commands[button] = command;
    }

    // User presses button -> execute assigned command.
    void pressButton(int button) {
        if (commands[button] != nullptr) {
            commands[button]->execute();
        }
    }
};


// ======================================================
// CLIENT
// ======================================================

int main() {

    // 1. Create Receiver
    TV tv;

    // 2. Create Commands
    Command* turnOn = new TurnOnCommand(&tv);
    Command* turnOff = new TurnOffCommand(&tv);
    Command* changeChannel = new ChangeChannelCommand(&tv, 5);
    Command* adjustVolume = new AdjustVolumeCommand(&tv, 20);

    // 3. Create Invoker
    RemoteControl remote(4);

    // 4. Configure buttons
    remote.setCommand(0, turnOn);
    remote.setCommand(1, turnOff);
    remote.setCommand(2, changeChannel);
    remote.setCommand(3, adjustVolume);

    // 5. User presses buttons
    remote.pressButton(0); // TV ON
    remote.pressButton(2); // Channel 5
    remote.pressButton(3); // Volume 20
    remote.pressButton(1); // TV OFF

    // Cleanup
    delete turnOn;
    delete turnOff;
    delete changeChannel;
    delete adjustVolume;

    return 0;
}
```

---

# 11. Complete Java Code

```java
import java.util.ArrayList;
import java.util.List;

// ======================================================
// COMMAND INTERFACE
// ======================================================

interface Command {
    void execute();
}


// ======================================================
// RECEIVER
// ======================================================

class TV {

    public void turnOn() {
        System.out.println("TV is ON");
    }

    public void turnOff() {
        System.out.println("TV is OFF");
    }

    public void changeChannel(int channel) {
        System.out.println("Channel changed to " + channel);
    }

    public void adjustVolume(int volume) {
        System.out.println("Volume set to " + volume);
    }
}


// ======================================================
// CONCRETE COMMANDS
// ======================================================

class TurnOnCommand implements Command {

    private TV tv;

    public TurnOnCommand(TV tv) {
        this.tv = tv;
    }

    @Override
    public void execute() {
        tv.turnOn();
    }
}


class TurnOffCommand implements Command {

    private TV tv;

    public TurnOffCommand(TV tv) {
        this.tv = tv;
    }

    @Override
    public void execute() {
        tv.turnOff();
    }
}


class ChangeChannelCommand implements Command {

    private TV tv;
    private int channel;

    public ChangeChannelCommand(TV tv, int channel) {
        this.tv = tv;
        this.channel = channel;
    }

    @Override
    public void execute() {
        tv.changeChannel(channel);
    }
}


class AdjustVolumeCommand implements Command {

    private TV tv;
    private int volume;

    public AdjustVolumeCommand(TV tv, int volume) {
        this.tv = tv;
        this.volume = volume;
    }

    @Override
    public void execute() {
        tv.adjustVolume(volume);
    }
}


// ======================================================
// INVOKER
// ======================================================

class RemoteControl {

    private List<Command> commands;

    public RemoteControl(int numberOfButtons) {
        commands = new ArrayList<>();

        for (int i = 0; i < numberOfButtons; i++) {
            commands.add(null);
        }
    }

    // Generic configuration.
    // Remote does not care whether command is TV, AC, speaker etc.
    public void setCommand(int button, Command command) {
        commands.set(button, command);
    }

    // Execute whatever command is assigned to this button.
    public void pressButton(int button) {
        if (commands.get(button) != null) {
            commands.get(button).execute();
        }
    }
}


// ======================================================
// CLIENT
// ======================================================

public class Main {

    public static void main(String[] args) {

        // 1. Create Receiver
        TV tv = new TV();

        // 2. Create Commands
        Command turnOn = new TurnOnCommand(tv);
        Command turnOff = new TurnOffCommand(tv);
        Command changeChannel = new ChangeChannelCommand(tv, 5);
        Command adjustVolume = new AdjustVolumeCommand(tv, 20);

        // 3. Create Invoker
        RemoteControl remote = new RemoteControl(4);

        // 4. Configure buttons
        remote.setCommand(0, turnOn);
        remote.setCommand(1, turnOff);
        remote.setCommand(2, changeChannel);
        remote.setCommand(3, adjustVolume);

        // 5. User presses buttons
        remote.pressButton(0); // TV ON
        remote.pressButton(2); // Channel 5
        remote.pressButton(3); // Volume 20
        remote.pressButton(1); // TV OFF
    }
}
```

---

# 12. Complete Flow

```text
Client
  |
  | creates TV
  ↓
Receiver: TV

Client
  |
  | creates command objects
  ↓
TurnOnCommand / TurnOffCommand / ChangeChannelCommand / ...
  |
  | each command stores reference to Receiver
  ↓
TV

Client
  |
  | assigns commands
  ↓
RemoteControl (Invoker)

User
  |
  | pressButton(0)
  ↓
RemoteControl
  |
  | command.execute()
  ↓
TurnOnCommand
  |
  | tv.turnOn()
  ↓
TV
```

Most important flow:

```text
User
 ↓
Remote (Invoker)
 ↓
Command.execute()
 ↓
Concrete Command
 ↓
Receiver (TV)
```

---

# 13. Why Command Pattern?

Without Command:

```text
Remote → directly knows TV actions
```

With Command:

```text
Remote → knows only Command
Command → knows the Receiver/action
```

So Remote does not need to know:

```text
TV?
AC?
Speaker?
Netflix?
Mute?
```

It only does:

```cpp
command->execute();
```

---

# 14. Command Pattern vs Strategy — Important

Dono mein interface + concrete classes dekhkar confusion ho sakta hai.

### Strategy

Question:

> **"Same task ko perform karne ka kaunsa algorithm use karna hai?"**

Example:

```text
Sort
 ├── QuickSort
 ├── MergeSort
 └── HeapSort
```

### Command

Question:

> **"Kaunsi request/action ko represent/store/execute karna hai?"**

Example:

```text
TurnOnCommand
TurnOffCommand
ChangeChannelCommand
```

Strategy = interchangeable behavior/algorithm.

Command = request/action as an object.

---

# 15. Key Points

1. Command is a **behavioral design pattern**.
2. Core idea: **request/action ko object bana do**.
3. `Command` interface usually has `execute()`.
4. Concrete Command specific action/request ko encapsulate karta hai.
5. Concrete Command usually Receiver ka reference rakhta hai.
6. Receiver actual work karta hai.
7. Invoker command ko invoke karta hai but actual action ka logic nahi jaanta.
8. Client objects create karta hai aur commands ko Invoker ke saath wire karta hai.
9. Command object ke andar action ke parameters bhi store kiye ja sakte hain.
10. Commands ko store, queue, delay, retry, log, etc. kiya ja sakta hai.
11. Generic Invoker design better hai than `setNetflixCommand()`, `setYouTubeCommand()`, etc.

---

# 16. One-Line Intuition

> **Command = "Kaam ko direct mat karo; kaam karne ki request ko object bana do."**

Example:

```text
"TV ON karo"
      ↓
TurnOnCommand object
      ↓
execute()
      ↓
TV.turnOn()
```

### Interview line

> **Command decouples the object that requests an operation (Invoker) from the object that performs it (Receiver) by encapsulating the request as a Command object.**
