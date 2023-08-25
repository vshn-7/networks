/**************************************************************
 * A Few Points we want to Add before running

1. We are not using absolute path
2. When you want to get a html file there is a warning regarding GTK, ignore it and type another command.
3. If the command is 'PUT http://127.0.0.1/:8080 a.pdf' it won't work 
   but  'PUT http://127.0.0.1:8080 a.pdf' will work perfectly.
4. In the Access_log file we are printing date and time 

***************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <time.h>
#include <poll.h>
#include <limits.h>

#define MAX 256


char* request(char* buf)
{
    char* request = malloc(1024*sizeof(char));
       // Construct the request
    memset(request, 0, sizeof(request));
    //char* p = strtok()
     char *a ;
     char* b;
     char k[100];
     int size;
    if(strstr(buf,"GET")!=NULL)
    {
        b = "GET";
          char *token = strtok(buf,"/");
          

            while(token != NULL)
            {
                //printf("%s\n",token);
               a = token;
            token = strtok(NULL, "/");
            }
            a = strtok(a,":");
            a[strlen(a)-1] = '\0';
          k[0] = '0';
    }
     else if (strstr(buf,"PUT") != NULL)
    {
        b = "PUT";
        char* token = strtok(buf, " ");
         token = strtok(NULL, " ");
         token = strtok(NULL," ");
        a = token;
        a[strlen(a)-1] = '\0';
          FILE *file = fopen(a, "r");
  if (!file) {
    perror("Could not open file");
  }

  fseek(file, 0, SEEK_END);
  size = ftell(file);
 // printf("SIZE : %d\n",size);
  fclose(file);
  sprintf(k,"%d",size);
 //printf("SIZE : %d\n",size);
    }
    
    //printf("FILE :%s\n",a);
    strcat(request,b);
    strcat(request," /");
    //strcat(request,"GET /");
    strcat(request,a);
    strcat(request," HTTP/1.1\r\n");
    //strcat(request, "GET / HTTP/1.1\r\n");
    strcat(request, "Host: localhost\r\n");
    strcat(request, "Connection: close\r\n");
    time_t now = time(NULL);
    char date[100];
    strftime(date, 100, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&now));
    strcat(request, "Date: ");
    strcat(request, date);
    strcat(request, "\r\n");
    //strcat(request, "Date: 2023-02-12\r\n");
    strcat(request,"Accept:");
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
    strcat(request,accept);
    //strcat(request, "Accept: text/html\r\n");
    strcat(request, "Accept-Language: en-us,en;q=0.8\r\n");
    time_t now1 = time(NULL);
    time_t modified_since = now1 - (2 * 24 * 60 * 60);
    char modified_since_str[100];
    strftime(modified_since_str, 100, "%a, %d %b %Y %H:%M:%S %Z", gmtime(&modified_since));
    strcat(request, "If-Modified-Since: ");
    strcat(request, modified_since_str);
    strcat(request, "\r\n");
    //strcat(request, "If-Modified-Since: Mon, 10 Feb 2023 08:48:01 GMT\r\n");
    strcat(request, "Content-Language: en-us\r\n");
    // int content_length = strlen(request_body);
    // char content_length_str[100];
    // sprintf(content_length_str, "%d", content_length);
    // strcat(request, "Content-Length: ");
    // strcat(request, content_length_str);
    // strcat(request, "\r\n");
    strcat(request, "Content-Length: ");
   //  printf("SIZE : %d\n",size);
    strcat(request,k);
    // printf("SIZE : %d\n",size);
    strcat(request,"\r\n");
    //strcat(request,)
    strcat(request,"Content-Type: ");
    strcat(request,accept);
    //strcat(request, "Content-Type: text/html\r\n");
    strcat(request, "\r\n");
    return request;

}


int main()
{
    
    while(1)
    {


        FILE * fr;
        printf("MyOwnBrowser>");
        int sockfd;
        char buffer[2000], input[MAX];
        struct sockaddr_in server_addr;

        sockfd = socket(AF_INET ,SOCK_STREAM, 0);
        if(sockfd == -1)
        {
            printf("Socket Not created\n");
        }

            struct pollfd mypoll;
            memset(&mypoll, 0, sizeof(mypoll));  
            mypoll.fd = sockfd;
            mypoll.events = POLLIN;
        //puts("Socket Created");
         char* buf1;
        fgets(input , MAX ,stdin);
        char* buf = strdup(input);
        char* buf2 = strdup(input);
        char* buf3 = strdup(input);
         char* buf4 = strdup(input);
        //printf("%s\n",input);
        // strtok(input , "/");
        // char* ip = strtok(NULL,"/");
        // //printf("IP : %s\n",ip);
        // char* path =  strtok(ip,":");
        // //printf("Path : %s\n",path);
        // char *port =strtok(NULL," ");
        // printf("%s\n",port);
        //port = strtok(port, " ");
        //printf("port:%s\n",port);
        char* ip = malloc(100*sizeof(char));
        char* port = malloc(100*sizeof(char));
        char* path = malloc(100*sizeof(char));
        int COU=0;
        int p=0;
        int x=0;
        int y=0;
        int col=0;
        int flag4=0;
        for(int i=0;input[i]!='\0' && input[i]!='\n';i++){
            if(input[i]=='/') COU++;
            if(input[i]==' ') flag4++;
            if(input[i]==':') col++;
            if(COU==2 && input[i]!='/' && flag4==1 && col==1){
                ip[p++]=input[i];
            }
            if(COU>=3 && col==1 && flag4==1 && col==1){
                path[x++]=input[i];
                if(x==1 && path[0]=='/') {x=0;}
            }
            if(col==2 && flag4==1 && input[i]!=':'){
                port[y++]=input[i];
            }
        }
        //printf("IP = %s port = %s  path = %s\n",ip,port,path);
       server_addr.sin_addr.s_addr = inet_addr(ip);
       server_addr.sin_family = AF_INET;
       if(y!=0)
        {
            server_addr.sin_port = htons(atoi(port));

        } 
        else server_addr.sin_port = htons(80);

        if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))< 0)
        {
            perror("connection failed .ERROR\n");
             printf("HTTP/1.1 400 Bad Request\n");

        }
        FILE* f1;
        char* a;

        if (strstr(buf4,"PUT") != NULL)
    {
        char* token = strtok(buf4, " ");
         token = strtok(NULL, " ");
         token = strtok(NULL," ");
        a = token;
    
    a[strlen(a)-1] = '\0';
    //printf("%s",a);
                f1 = fopen(a,"r");
        if (f1 == NULL) {
            printf("File %s not found on client\n", a);
            printf( "HTTP/1.1 403 Not Found\n");
            close(sockfd);
            continue;
        }

    }

        char* req = request(buf3);
        //strcpy(buf1,req);
        send(sockfd,req,strlen(req), 0);
        char b[10];
         recv(sockfd, b, 10, 0);
        send(sockfd,buf,strlen(buf), 0);

        if(strstr(buf4,"GET") != NULL)
        {
            char c[30];
            memset(c,0x00,30);
            recv(sockfd,c,30,0);
            char e[5];
            memset(e,0x00,5);
            for(int i =0 ; i < 5;i++)
            {
                e[i] = c[i];
            }
        //    printf("%s\n",e);
        //    printf("%s\n",c);
            if(strcmp(c,"Found") != 0)
         { 
          printf("HTTP/1.1 404 Not Found\n");
          //printf("aaaaaa");
            close(sockfd);
            continue;
             }
           
         send(sockfd,"OK",2 ,0);
        }
        //memset(buf,0,sizeof(buf));
if(strstr(buf,"GET")!=NULL){
    char* fname;
    char* f;
    f = strtok(path,"/");
    while(f!=NULL){
        fname = f;
        f = strtok(NULL,"/");
    }
   //fname[strlen(fname)-1]='\0';
    //printf("fname : %s\n",fname);
        int flag = 0;
            if(fname[strlen(fname)- 1] == 't' || fname[strlen(fname)- 1] == 'f' || fname[strlen(fname)- 1] == 'l' || fname[strlen(fname)- 1] == 'g')
    {
          flag = 1;
    }
    if(flag != 1)
    {
        printf("HTTP/1.1 400 Bad Request\n");
        continue;
    }
   if(fname[strlen(fname)- 1] == 't')
   {

        char response[2000];
        if(poll(&mypoll, 1, 3000) > 0){
           recv(sockfd, response,2000,0);
            printf("%s",response);
        } else{
            printf("Response Timed Out\n");
            close(sockfd);
            continue;
        }
    

     fr = fopen(fname, "w");
    if (fr == NULL) {
        printf("File %s Cannot be Downloaded.\n", fname);
        return 1;
    }
    int b;
    while ((b = recv(sockfd, buffer, 2000, 0)) > 0) {
        fwrite(buffer, 1, b, fr);
    }
    printf("Downloaded file %s from server\n", fname);
    fclose(fr);
      char command[1024];
  //PATH[m-1] = '\0';
  sprintf(command, "xdg-open %s",fname);
  system(command);   
   } 
   else if (fname[strlen(fname)- 1] == 'f' || fname[strlen(fname)- 1] == 'g')
   {
    char response[2000];
        if(poll(&mypoll, 1, 3000) > 0){
           recv(sockfd, response,2000,0);
            printf("%s",response);
        } else{
            printf("Response Timed Out\n");
            close(sockfd);
            continue;
        }
            //printf("/nABHI/n");
    //printf("1\n");
         int fd;
    char *pdf_header = "%PDF-1.7\n\n";

    fd = open(fname, O_CREAT | O_WRONLY, 0644);
    if (fd == -1)
    {
        perror("Error opening file");
        return 1;
    }

    write(fd, pdf_header, strlen(pdf_header));
    close(fd);
     FILE *pdf_file = fopen(fname, "wb");
    if (!pdf_file) {
        perror("ERROR opening received file");
    }
   int n;
    while ((n = recv(sockfd, buffer, 2000,0)) > 0) {
        if (fwrite(buffer, sizeof(char), n, pdf_file) != n) {
            perror("ERROR writing to file");
        }
    }

    if (n < 0) {
        perror("ERROR reading from socket");
    }

    printf("Received PDF file successfully\n");
     fclose(pdf_file);
    //  char z[10] = "REcieved";
    //  send(sockfd,z,10,1);
    //   char a[] = "OK";
    //  send(sockfd,a, strlen(a), 0);
           
    close(sockfd);
   
  char command[100];
  //PATH[m-1] = '\0';
  sprintf(command, "xdg-open %s",fname);
  system(command);
   }
   else if(fname[strlen(fname)- 1] == 'l')
   {
        char response[2000];
        if(poll(&mypoll, 1, 3000) > 0){
           recv(sockfd, response,2000,0);
            printf("%s",response);
        } else{
            printf("Response Timed Out\n");
            close(sockfd);
            continue;
        }
       fr = fopen(fname, "w");
    if (fr == NULL) {
        printf("File %s Cannot be Downloaded.\n", fname);
        return 1;
    }
    int b;
    while ((b = recv(sockfd, buffer, 2000, 0)) > 0) {
        fwrite(buffer, 1, b, fr);
    }
    printf("Downloaded file %s from server\n", fname);
    fclose(fr);

//    char *result = realpath(fname, NULL);
//    if (result) {
//       char command[1024];
//       sprintf(command, "xdg-open %s", fname);
//       system(command);
//       free(result);
      
//    }

   char cwd[1000];
   char command[1024];
   char *result = getcwd(cwd, sizeof(cwd));
   strcat(cwd, "/");
    strcat(cwd, fname);
   sprintf(command, "xdg-open %s", cwd);
   system(command);
   close(sockfd);
   }
    }
    
     else if(strstr(buf,"PUT") != NULL)
    {
        //printf("1\n");
        char* fx = strtok(buf2," ");
            char* x = strtok(NULL," ");
            char* fname = strtok(NULL," ");
        fname[strlen(fname)-1]='\0';
        //printf("%s",fname);
        int flag = 0;
            if(fname[strlen(fname)- 1] == 't' || fname[strlen(fname)- 1] == 'f' || fname[strlen(fname)- 1] == 'l' || fname[strlen(fname)- 1] == 'g')
    {
          flag = 1;
    }
    if(flag != 1)
    {
        printf("HTTP/1.1 400 Bad Request\n");
        continue;
    }

       if(fname[strlen(fname)-1] == 't' || fname[strlen(fname)-1] == 'l' )
       {
        //printf("1\n");
  

          char response[2000];
        if(poll(&mypoll, 1, 3000) > 0){
           recv(sockfd, response,2000,0);
            printf("%s",response);
        } else{
            printf("Response Timed Out\n");
            close(sockfd);
            continue;
        }
       FILE * f = fopen(fname,"r");
        if (f == NULL) {
            printf("File %s not found on client\n", fname);
            printf( "HTTP/1.1 404 Not Found\n");
            continue;
        } else {
            // Send the contents of the file to the client
            while (fgets(buffer, 2000, f) != NULL) {
                send(sockfd, buffer, strlen(buffer), 0);
            }
            fclose(f);
            close(sockfd);
        }

    } else if(fname[strlen(fname)-1] == 'f' || fname[strlen(fname)-1] == 'g')
    {
        //printf("1\n");
            char response[2000];
        if(poll(&mypoll, 1, 3000) > 0){
           recv(sockfd, response,2000,0);
            printf("%s",response);
        } else{
            printf("Response Timed Out\n");
            close(sockfd);
            continue;
        }
                    FILE *pdf_file;
                pdf_file = fopen(fname, "rb");
            if (!pdf_file) {

             printf("File %s not found on client\n", fname);
            printf( "HTTP/1.1 404 Not Found\n");
            continue;
            }

    // Read the PDF file and send it to the client
            int bytes_read;
            while ((bytes_read = fread(buffer, 1, sizeof(buffer), pdf_file)) > 0) {
                send(sockfd, buffer, bytes_read, 0);
            }
                fclose(pdf_file);



                close(sockfd);
    }
    }
    free(req);
    free(ip);
   free(port);
   free(path);
    fflush(stdin);
   //ree(request);

    }
}