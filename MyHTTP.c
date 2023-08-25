/**************************************************************
 * A Few Points we want to Add before running

1. We are not using absolute path
2. When you want to get a html file there is a warning regarding GTK, ignore it and type another command.
3. If the command is 'PUT http://127.0.0.1/:8080 a.pdf' it won't work 
   but  'PUT http://127.0.0.1:8080 a.pdf' will work perfectly.
4. In the Access_log file we are printing date and time 

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

char* response(char* buf)
{
    char* response = malloc(1024*sizeof(char));
    char* buf1 = strdup(buf);
     char *a ;
     char* b;
     int size;
     char k[10];
    if(strstr(buf,"GET")!=NULL)
    {
        b = "GET";
          
          char* token1 = strtok(buf1,"/");
          token1 = strtok(NULL, "/");
          token1 = strtok(NULL,":");
          //printf("%s\n",token1);
          token1[strlen(token1)-1] = '\0';
          FILE *file = fopen(token1, "r");
  if (!file) {
    perror("Could not open file");
  }

  fseek(file, 0, SEEK_END);
  size = ftell(file);
 // printf("SIZE : %d\n",size);
  fclose(file);
  sprintf(k,"%d",size);
 //printf("SIZE : %d\n",size);
       char *token = strtok(buf,"/");     
            while(token != NULL)
            {
                //printf("%s\n",token);
               a = token;
            token = strtok(NULL, "/");
            }
            a = strtok(a,":");
          a[strlen(a)-1] = '\0';
         // printf("%s\n",a);
    }
     else if (strstr(buf,"PUT") != NULL)
    {
        b = "PUT";
        char* token = strtok(buf, " ");
         token = strtok(NULL, " ");
         token = strtok(NULL," ");
        a = token;
        a[strlen(a)-1] = '\0';
        k[0]='0';
    }

        char* accept;
    if(a[strlen(a)-1] == 'f' || a[strlen(a)-1] == 'd')
    {
           accept = "application/pdf\r\n";
    } else if (a[strlen(a)-1] == 'l' || a[strlen(a)-1] == 'm')
    {
            accept ="text/html\r\n";
    }else if (a[strlen(a)-1] == 'g' || a[strlen(a)-1] == 'e' || a[strlen(a)-1] == 'p') 
    {
            accept = "image/jpeg\r\n";
    }else
    {
            accept = "text/*\r\n";
    }
    
strcpy(response, "HTTP/1.1 200 OK\r\n");

time_t now = time(NULL);
time_t expire = now + (3 * 24 * 60 * 60);
char expires[100];
strftime(expires, 100, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&expire));
strcat(response, "Expires: ");
strcat(response, expires);
strcat(response, "\r\n");

strcat(response, "Cache-Control: no-store\r\n");

strcat(response, "Content-Language: en-us\r\n");
strcat(response, "Content-Length: ");
     //printf("SIZE : %d\n",size);
    strcat(response,k);
    // printf("SIZE : %d\n",size);
    strcat(response,"\r\n");
//strcat(response, "Content-Length: 0\r\n");
    strcat(response,"Content-Type: ");
    strcat(response,accept);
// int content_length = strlen(response_body);
// char content_length_str[100];
// sprintf(content_length_str, "%d", content_length);
// strcat(response, "Content-Length: ");
// strcat(response, content_length_str);
// strcat(response, "\r\n");
//strcat(response, "Content-Type: text/html\r\n");
// strcat(response, "Content-Type: ");
// strcat(response, content_type);
// strcat(response, "\r\n");

char last_modified[100];
strftime(last_modified, 100, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&now));
strcat(response, "Last-Modified: ");
strcat(response, last_modified);
strcat(response, "\r\n");
//printf("%s",response);
return response;
// strcat(response, "\r\n");
// strcat(response, response_body);

}



void log_access(char client_ip[], int client_port, char* buf3)
{
    
   FILE *FR;
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    char date_time[20];
    strftime(date_time, sizeof(date_time), "%Y%m%d%H%M%S", tm);
    char date[10];
    int d=0;
    char time[10];
    int ti=0;
    for(int i=0;i<14;i++){
        if(i>1 && i<8){
            date[d++]=date_time[i];
        }
        else if(i>=8){
            time[ti++]=date_time[i];
        }
    }
    char *A = strtok(buf3," ");
    char *B = strtok(NULL," ");
    FR = fopen("Access_log.txt","a");
        if (FR == NULL) {
        printf("Failed to open the file\n");
        return;
    }
    time[6] = '\0';
    date[6] = '\0';
    // printf("%s\n",date);
    // printf("%s\n",time);

    fprintf(FR,"%s:%s:%s:%d:%s:%s\n",date,time,client_ip,client_port,A,B);
    // fprintf(FR,date);
    // fprintf(FR,":");
    // fprintf(FR,time);
    // fprintf(FR,":");
    // fprintf(FR,client_ip);
    // fprintf(FR,":");
    // fprintf(FR,"%d",client_port);
    // fprintf(FR,":");
    // fprintf(FR,A);
    // fprintf(FR,":");
    // fprintf(FR,B);
    // fprintf(FR,"\r\n");
    fclose(FR);  
}

int main()
{
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char buffer[2000];
    char file_name[2000];
    char request[20000];
    FILE *fr;
    FILE *FR;
    // Create the socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        printf("Could not create socket");
    }
    puts("Socket created");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(80);

    // Bind
    if (bind(socket_desc, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc, 3);

while(1)
{

// printf("1\n");
    // Accept and incoming connection
    //puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t *) &c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }

    puts("Connected to client");
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client.sin_port);
    //printf("Client connected from %s:%d\n", client_ip, client_port);
    // Receive the GET request from the client
     


    int p=fork();
 if(p==0){
    close(socket_desc);



    memset(request, 0, 1000);
    read_size = recv(client_sock, request, 1000, 0);
    printf("%s",request);
    char a[] = "OK";

     send(client_sock,a, strlen(a), 0);
    
    memset(buffer, 0, 100);
    read_size = recv(client_sock, buffer, 100, 0);

    //printf("%s\n",buffer);
    if (read_size == 0) {
        puts("Client disconnected");
        fflush(stdout);
    } else if (read_size == -1) {
        perror("recv failed");
    }



    char* ip = malloc(100*sizeof(char));
        char* port = malloc(100*sizeof(char));
        char* path = malloc(100*sizeof(char));
        int COU=0;
        int p=0;
        int x=0;
        int y=0;
        int col=0;
        int flag4=0;
        for(int i=0;buffer[i]!='\0' && buffer[i]!='\n';i++){
            if(buffer[i]=='/') COU++;
            if(buffer[i]==' ') flag4++;
            if(buffer[i]==':') col++;
            if(COU==2 && buffer[i]!='/' && flag4==1 && col==1){
                ip[p++]=buffer[i];
            }
            if(COU>=3 && col==1 && flag4==1 && col==1){
                path[x++]=buffer[i];
                if(x==1 && path[0]=='/') {x=0;}
            }
            if(col==2 && flag4==1 && buffer[i]!=':'){
                port[y++]=buffer[i];
            }
        }
        //printf("IP = %s port = %s  path = %s\n",ip,port,path);
   
    char* buf = strdup(buffer);
    char* buf2 = strdup(buffer);
    char* buf3 = strdup(buffer);
    char* buf4 = strdup(buffer);
     char* buf5 = strdup(buffer);
    if(strstr(buf4,"GET")!=NULL)
    {
    //     //printf("%s\n",buffer);
    //     strtok(buffer , "/");
    //     char* ip = strtok(NULL,"/");
    //     //printf("IP : %s\n",ip);
    //    path =strtok(NULL,":");
    //    // path[strlen(path)-1] ='\0';
       
    //     char *port =strtok(NULL,":");
    //     //path[strlen(path)-1] ='\0';
       // printf("%s",path);

             FILE * ab;
             ab = fopen(path,"rb");
        

        if (ab == NULL) {
            printf("File %s not found on server\n", path);
            send(client_sock, "HTTP/1.1 404 Not Found\n", 23, 0);
            continue;
        } else send(client_sock, "Found", 5, 0);
          
          fclose(ab);
          char k[5];
          recv(client_sock,k,5,0);
    }


    if(strstr(buf,"GET")!=NULL){
            //path[strlen(path)-1] ='\0';
           // printf("%s %d\n",path,strlen(path));
       
        if(path[strlen(path)-1] == 't' )
       {

            
            char * resp;
              resp =  response(buf5);
              send(client_sock,resp,strlen(resp),0);

            fr = fopen(path,"r");
        

        if (fr == NULL) {
            printf("File %s not found on server\n", path);
            send(client_sock, "HTTP/1.1 404 Not Found\n", 23, 0);
        } else {
            // Send the contents of the file to the client
            while (fgets(buffer, 2000, fr) != NULL) {
                send(client_sock, buffer, strlen(buffer), 0);
            } 
        
            fclose(fr);
            close(client_sock);
        }
        } else if (path[strlen(path)-1] == 'f' || path[strlen(path)-1] == 'g' )
        {
             char * resp;
              resp =  response(buf5);
              send(client_sock,resp,strlen(resp),0);
            FILE *pdf_file;
                pdf_file = fopen(path, "rb");
            if (!pdf_file) {
                printf("Failed to open PDF file: %s\n",path );
                return 1;
            }

    // Read the PDF file and send it to the client
            int bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), pdf_file)) > 0) {
                send(client_sock, buffer, bytes_read, 0);
            }
                fclose(pdf_file);
            free(resp);
                
    //char* r;
        //    char z[10] ;
        //    recv(client_sock,z,10,0);

      
           // printf("HIIII");
        //              char a[10];
        //  recv(client_sock, a, 10, 0);
        
        /// printf("%s/n",a);

              //printf("/nABHI/n");
              //printf("%s",resp);
                
       
        close(client_sock);
        }   
        else if(path[strlen(path)-1] == 'l')
        {
             char * resp;
              resp =  response(buf5);
              send(client_sock,resp,strlen(resp),0);
    // Read the contents of a HTML file
              fr = fopen(path,"r");
        

        if (fr == NULL) {
            printf("File %s not found on server\n", path);
            send(client_sock, "HTTP/1.1 404 Not Found\n", 23, 0);
        } else {
            // Send the contents of the file to the client
            while (fgets(buffer, 2000, fr) != NULL) {
                send(client_sock, buffer, strlen(buffer), 0);
            }
        
            fclose(fr);

            
            close(client_sock);
        }

        }
        }
        else if(strstr(buf,"PUT") != NULL)
        {
            char* fx = strtok(buf2," ");
            char* x = strtok(NULL," ");
            char* fname = strtok(NULL," ");
            fname[strlen(fname)-1] = '\0';

            
            char* Fname = strdup(fname);
        //     strtok(x,"/");
        //    char* z =  strtok(NULL,"/");
        //     printf("%s\n",z);
        //     char* y=strtok(NULL,"/");
        //     char* PATH = malloc(100*sizeof(char));
        //     if(y == NULL)
        //     {
        //         printf("abhi \n");
        //         strcat(PATH,Fname);
        //     }
        //     printf("%s\n",PATH);
        //     if(y != NULL)
        //     {

        //     PATH   = strtok(y,":");
        //     PATH = strtok(PATH," ");
        // if(PATH!=NULL){

        //      strncat(PATH,"/ab",1);
        //      printf("%s",fname);
        //      strcat(PATH,Fname);
        // }
        //     }
        //     printf("%s\n",PATH);
            
            //PATH[strlen(PATH)-1] = '\0';
            //printf("%s\n",PATH);

            
            char* j = strtok(x,":");
            char* I = strtok(NULL,":");
    
           int count =0;
           for(int i=0;i<strlen(I);i++){
            if(I[i]=='/') count++;
           }
            char* PATH = malloc(100*sizeof(char));
            memset(PATH,0x00,100);
            if(count==2){
                strcat(PATH,Fname);
            }
            else{
                strtok(I,"/");
                char* P = strtok(NULL," ");
                //printf("%s\n",P);
                strcat(PATH,P);
                strcat(PATH,"/");
                strcat(PATH,Fname);
            }
            int m = strlen(PATH);
            
            //printf("%d\n",m);
            //PATH[strlen(PATH)-2] = "\0";

    if(fname[strlen(fname)-1] == 't' || fname[strlen(fname)-1] == 'l')
    {

                char * resp;
              resp =  response(buf5);
              send(client_sock,resp,strlen(resp),0);

    // printf("1\n");
            FILE * f = fopen(PATH, "w");
             //PATH[m-1] = '\0';
            //printf("%s\n",PATH);
            
    if (f == NULL) {
        printf("File %s Cannot be Downloaded.\n", PATH);
        return 1;
    }
    int b;
     
    while ((b = recv(client_sock, buffer, 2000, 0)) > 0) {
        fwrite(buffer, 1, b, f);
    }
    fclose(f);
    close(client_sock);
//   char command[100];
//   //PATH[m-1] = '\0';
//   sprintf(command, "xdg-open %s",PATH);
//   system(command);
    } else if(fname[strlen(fname)-1] == 'f' || fname[strlen(fname)-1] == 'g')
    {
        // printf("1\n");
        // printf("1\n");

                     char * resp;
              resp =  response(buf5);
              send(client_sock,resp,strlen(resp),0);
         int fd;
    char *pdf_header = "%PDF-1.7\n\n";
      //printf("%s\n",PATH);
    fd = open(PATH, O_CREAT | O_WRONLY, 0644);
    if (fd == -1)
    {
        perror("Error opening file");

        // return 1;
    }

    write(fd, pdf_header, strlen(pdf_header));
    close(fd);
     FILE *pdf_file = fopen(PATH, "wb");
    if (!pdf_file) {
        perror("ERROR opening received file");
    }
   int n;
    while ((n = recv(client_sock, buffer, 2000,0)) > 0) {
        if (fwrite(buffer, sizeof(char), n, pdf_file) != n) {
            perror("ERROR writing to file");
        }
    }

    if (n < 0) {
        perror("ERROR reading from socket");
    }

    printf("Received PDF file successfully\n");

    close(client_sock);
    fclose(pdf_file);
    }
        } 
        log_access(client_ip,client_port,buf3);
    for(int i=0;i<strlen(buf);i++) buf[i]='\0';
   
   free(ip);
   free(port);
   free(path);
   fflush(stdin);
   close(client_sock);
   exit(0);
}
close(client_sock);
}
            return 0;
}
