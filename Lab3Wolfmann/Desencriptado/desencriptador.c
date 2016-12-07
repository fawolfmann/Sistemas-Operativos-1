#include <linux/kernel.h>       
#include <linux/module.h>       
#include <linux/fs.h>			//para usar funciones de dispositivo caracter
#include <asm/uaccess.h>        // para usar get_user y put_user 

#include "desencriptador.h"
#define SUCCESS 0
#define NOMBRE "desencriptador"
#define BUF_LEN 80

static int Device_Open = 0; //Esta bandera se usa para prevenir el acceso concurrente al dispositivo
static char Message[BUF_LEN]; //El mensaje que nos da cuando se le pide
static char *Message_Ptr;

int device_open(struct inode *inode, struct file *file)
{
	if (Device_Open) //solo un proceso a la vez
                return -EBUSY;

        Device_Open++;
    
        Message_Ptr = Message; //comienza el mensaje
        try_module_get(THIS_MODULE);
        return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
        Device_Open--; //se le permite a otro tomar el dispositivo

        module_put(THIS_MODULE);
        return SUCCESS;
}


static ssize_t device_read(struct file *file, char __user * buffer,size_t length,loff_t * offset)
{
        int bytes_read = 0; //cantidad de bytes escritas
        
        if (*Message_Ptr == 0) //si se llega al final del mensaje, devuelve 0
                return 0;
                
		//ingresa los datos al buffer
        while (length && *Message_Ptr) {
        	
                put_user(*(Message_Ptr++), buffer++);
                length--;
                bytes_read++;
        }
        
        return bytes_read;
}

static ssize_t device_write(struct file *file, char __user * buffer,size_t length,loff_t * offset)
{
        int i;
        
        for (i = 0; i < length && i < BUF_LEN; i++)
                get_user(Message[i], buffer + i);
        
        //Este for es el que toma los caracteres ingresados por el usuario y les suma 1 para encriptarlos.
        for (i = 0; i < length && i < BUF_LEN; i++)
				Message[i] = Message[i]+1;

        Message_Ptr = Message;
        
        return i;
}

int device_ioctl(struct inode *inode,   /* see include/linux/fs.h */
                 struct file *file,     /* ditto */
                 unsigned int ioctl_num,        /* number and param for ioctl */
                 unsigned long ioctl_param)
{
        int i;
        char *temp;
        char ch;
        
        switch (ioctl_num) {
        case IOCTL_SET_MSG:
                temp = (char *)ioctl_param;
                
                get_user(ch, temp);
                for (i = 0; ch && i < BUF_LEN; i++, temp++)
                        get_user(ch, temp);

                device_write(file, (char *)ioctl_param, i, 0);
                break;

        case IOCTL_GET_MSG:
        	
                i = device_read(file, (char *)ioctl_param, 99, 0);
                
                put_user('\0', (char *)ioctl_param + i);
                break;

        case IOCTL_GET_NTH_BYTE:
        	
                return Message[ioctl_param];
                break;
        }
        
        return SUCCESS;
}

//Para guardar las operaciones realizadas en el driver
struct file_operations Fops = {
        .read = device_read,
        .write = device_write,
        .unlocked_ioctl = device_ioctl,
        .open = device_open,
        .release = device_release,  
};

int init_module()
{
        int ret_val;
        ret_val = register_chrdev(MAJOR_NUM, NOMBRE, &Fops);

        if (ret_val < 0) { //negativo significa error
                printk(KERN_ALERT "%s Fallo con codigo de error: %d\n", ret_val);
                return ret_val;
        }
}


void cleanup_module()
{
	unregister_chrdev(MAJOR_NUM, NOMBRE);	
}

