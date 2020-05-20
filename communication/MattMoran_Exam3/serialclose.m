s = serial('COM10', 'Baudrate', 9600);
for i = 1:12
    delete(s);
end
