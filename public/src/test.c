#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "thread_pool.h"
#include "LibLoger.h"
#include <pthread.h>




tp_thread_pool *g_threadpool;


void *thread_fun(void *param)
{
	int i;
	pthread_t curid;			//current thread id

	info("aaaaa\n");
	
	
	//get current thread id
	curid = pthread_self();
	for(i=0;i<100;i++)
	{
		info("i=%d,thread id = 0x%X,param = %d\n",i,curid,(int)param);
		sleep(1);
	}
	return NULL;
}


pthread_mutex_t mutex ;
void *print_msg(void *arg){
        int i=0;
        //pthread_mutex_lock(&mutex);
        for(i=0;i<15;i++){
                printf("output : %d\n",i);
                usleep(100);
        }
        //pthread_mutex_unlock(&mutex);
}

int main(void)
{
	/* a bunch of json: */
/* 	char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";
	char text2[]="[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[]="[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[]="{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[]="[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";

    char text6[] = "<!DOCTYPE html>"
        "<html>\n"
        "<head>\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "  <style type=\"text/css\">\n"
        "    html, body, iframe { margin: 0; padding: 0; height: 100%; }\n"
        "    iframe { display: block; width: 100%; border: none; }\n"
        "  </style>\n"
        "<title>Application Error</title>\n"
        "</head>\n"
        "<body>\n"
        "  <iframe src="//s3.amazonaws.com/heroku_pages/error.html">\n"
        "    <p>Application Error</p>\n"
        "  </iframe>\n"
        "</body>\n"
        "</html>\n";

	char *out;
	cJSON *json;
	cJSON *jsonTemp;
	
	json = cJSON_CreateObject();
	cJSON_AddStringToObject(json,"item1","strings1");
	cJSON_AddNumberToObject(json,"item2",234);
	cJSON_AddBoolToObject(json,"item3",1);
	

	//json=cJSON_Parse(text1);
	jsonTemp = json;
	int i=0;
	
	while(jsonTemp != NULL)
	{
		printf("print %d\n",++i);
		printf("json type: %d\n",jsonTemp->type);
		printf("json valuestring: %s\n",jsonTemp->valuestring);
		printf("json valueint: %d\n",jsonTemp->valueint);
		printf("json valuedouble: %lf\n",jsonTemp->valuedouble);
		printf("json valuedouble: %s\n",jsonTemp->string);
		
		
		//printf("json cJSON_GetObjectItem: %s\n",cJSON_GetObjectItem(jsonTemp,"format"));
		jsonTemp = cJSON_GetObjectItem(jsonTemp,"format");
		if(jsonTemp!=NULL)
			printf("%s\n",cJSON_Print(jsonTemp));
	}
	
	
	
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
	 */
	
/* 	g_threadpool = creat_thread_pool(3,10);
	g_threadpool->init(g_threadpool);
	printf("111111111111");
	g_threadpool->process_job(g_threadpool,thread_fun,(void*)1);
	sleep(1);
	printf("2222222222222");
	g_threadpool->process_job(g_threadpool,thread_fun,(void*)2);
	sleep(1);
	g_threadpool->process_job(g_threadpool,thread_fun,(void*)3);
	sleep(1);
	g_threadpool->process_job(g_threadpool,thread_fun,(void*)4);

	sleep(10);
	

	while(1)
	{
		sleep(2);
	}
	
	g_threadpool->close(g_threadpool); */
	


        pthread_t id1;
        pthread_t id2;
        pthread_mutex_init(&mutex,NULL);
        pthread_create(&id1,NULL,print_msg,NULL);
        pthread_create(&id2,NULL,print_msg,NULL);
        pthread_join(id1,NULL);
        pthread_join(id2,NULL);
        pthread_mutex_destroy(&mutex);
        return 1;


	
	
}



