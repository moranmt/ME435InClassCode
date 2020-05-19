s = serial('COM10', 'Baudrate', 9600);
fopen(s);
fprintf('Running\n');
for i = 1:5
    fprintf('Turning LED ON\n');
    fprintf(s,'ON\n');
    pause(1);
    fprintf('Turning LED OFF\n');
    fprintf(s,'OFF\n');
    pause(1);
end

fclose(s);