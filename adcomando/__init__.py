import time
import serial

import pycomando


def show(bs):
    print("[echo]->%r" % bs)


class AutoDriverComando(object):
    def __init__(self, port='COM5', rate=9600, board_ind=0):
        self.board_ind = board_ind
        self.con = serial.Serial(port, rate)
        time.sleep(1)
        self.con.setDTR(level=0)
        time.sleep(1)
        self.com = pycomando.Comando(self.con)
        self.text = pycomando.protocols.TextProtocol(self.com)
        self.cmd = pycomando.protocols.CommandProtocol(self.com)
        self.com.register_protocol(0, self.text)
        self.com.register_protocol(1, self.cmd)
        self.text.receive_message = self.show

    def show(self, bs):
        print("[echo]->%r" % bs)

    def configure(self):
        """
        reconfigures board with updated settings
        """
        self.cmd.send_command(0, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def soft_stop(self):
        """
        brings the motor to a soft stop
        """
        self.cmd.send_command(1, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def hard_stop(self):
        """
        brings the board to a hard stop
        """
        self.cmd.send_command(2, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def release(self):
        """
        releases the board
        """
        self.cmd.send_command(3, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def set_max_speed(self, max_sp):
        """
        sets the max speed to integer value specified
        """
        if type(max_sp) != int:
            max_sp = int(max_sp)
        self.cmd.send_command(4, (self.board_ind, max_sp))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def set_accel(self, accel):
        """
        sets accel and decell to value specified
        """
        if type(accel) != int:
            accel = int(accel)
        self.cmd.send_command(5, (self.board_ind, accel))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def set_current(self, k_val):
        """
        sets the k value (must be int between 0 and 255)
        """
        if type(k_val) != int:
            k_val = int(k_val)
        self.cmd.send_command(6, (self.board_ind, k_val))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def set_microstepping(self, ms):
        """
        sets the microstepping value int power
        i.e 7 = 2^7
        """
        if type(ms) != int:
            ms = int(ms)
        self.cmd.send_command(7, (self.board_ind, ms))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def low_speed_mode(self, enabled):
        """
        sets low speed mode to on or off
        """
        if type(enabled) != bool:
            enabled = bool(enabled)
        self.cmd.send_command(8, (self.board_ind, enabled))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def is_moving(self):
        """
        checks if board is moving
        """
        self.cmd.send_command(9, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def wait(self):
        """
        waits until the arduino is done with its current operation
        """
        self.cmd.send_command(10, (self.board_ind, ))
        time.sleep(.5)
        while self.con.inWaiting():
            self.com.handle_stream()

    def rotate(self, dir, sps):
        """
        rotates the motor in the direction of dir at sps steps per second
        """
        if type(dir) != int:
            dir = int(dir)
        if type(sps) != int:
            sps = int(sps)
        self.cmd.send_command(11, (self.board_ind, dir, sps))
        time.sleep(.5)
        while self.con.inWaiting():
            print(self.con.readline())

    def move_steps(self, dir, steps):
        """
        moves number of steps in the direction specified
        """
        if type(dir) != int:
            dir = int(dir)
        if type(steps) != int:
            steps = int(steps)
        self.cmd.send_command(12, (self.board_ind, dir, steps))
        time.sleep(.1)
        while self.con.inWaiting():
            self.com.handle_stream()
