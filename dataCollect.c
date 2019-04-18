/*
 * 
 * Author: Jacob Vallee
 * 
 * ECE198 - High altitude ballooning
 *
 * Description:
 * This file contains the code used to collect data from the MPL3115A2.
 * The MPL3115A2 collects data on the tempurature, barometric presure and
 * altitude. This data is sent to the raspberry pi via i2c. This data will be
 * stored, and used to predict the decent time of the parachutte.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

void clr(void);
void cntr(void);

int main(int argc, char *argv[]){

        int fp; //File pointer
	int i = 0; //Counter
        char setup[2] = {0};
        char data[6] = {0};
        char reg[1] = {0x00};
        int height;
        int temp;
        float alt;
        float tempf;
        float tempc;
        int tpres;
        float press;

        FILE *fd; // for data file

        
        while(1){

		if((fp = open("/dev/i2c-1", O_RDWR)) < 0){
			printf("Error failed to open the i2c bus\n");
			exit(1);
		}

		/* i2c slave address is 0x60 */
		ioctl(fp, I2C_SLAVE, 0x60);
		/*Select the data config register (0x13) */
		/*Enable data ready event + flags */
		setup[0] = 0x13;
		setup[1] = 0x07;
		write(fp, setup, 2);

		/* Set altitude offset */
		//setup[0] = 0x2D;
		//setup[1] = 0x00;
		//write(fp, setup, 2);
		if(i==0){
			fd = fopen("balloondat.txt", "w+");
		}

		if(i!=0){
			fd = fopen("balloondat.txt","a+");
		}
		i = 1;

                /* Select control register */
                /* Active, OSR = 128 */
                /* altimeter mode (0xB9) */
                setup[0] = 0x26;
                setup[1] = 0xB9;
                write(fp, setup, 2);
                sleep(1);

                /* Read 6 bytes of data from the 0x00 Register */
                /* status, height msb1, height msb, height lsb, temp msb, temp lsb */
                write(fp, reg, 1);
                if(read(fp, data, 6) != 6){
                        printf("Error reading data.\n");
                        exit(1);
                }

                /* Convert the 6 bytes of data */
                height = ((data[1] * 65536) + (data[2] * 256 + (data[3] & 0xF0)) / 16);
                temp = ((data[4] * 256) + (data[5] & 0xF0)) / 16;
                alt = height / 16;
		alt = alt / 25480; // Manual offset for some reason the altitude is way too high....
                tempc = temp / 16;
                tempf = tempc * 1.8 + 32;

                /* Select the control register (0x26) */
                /* Set to barometer mode (0x39) */
                setup[0] = 0x26;
                setup[1] = 0x39;
                write(fp, setup, 2);
                sleep(1);

                /* Now read 4 bytes of data from 0x00 */
                /* status, pres msb1, pres msb, pres lsb */
                write(fp, reg, 1);
                read(fp, data, 4);

                /* Convert the data */
                tpres = ((data[1] * 65536) + (data[2] * 256 + (data[3] & 0xF0))) / 16;
                press = (tpres / 4) / 1000;

                //system("clear");
                /* Print the Data */
                
                fprintf(fd, "%.2f kPa\t %.2f m\t %.2f C\t %.2f F\n", press, alt, tempc, tempf);
                printf("\tPressure\t: %.2f kPa\n", press);
                printf("\tAltitude\t: %.2f m\n", alt);
                printf("\tTemp Celsius\t: %.2f C\n", tempc);
                printf("\tTemp Fahrenheit\t: %.2f F\n", tempf);
		sleep(3);
		fclose(fd);
		close(fp);
        }
        return 0;

}

void cntr(void){
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void clr(void){
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}
