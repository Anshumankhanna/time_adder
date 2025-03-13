// a program that adds the total time given in hh:mm:ss format

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct {
	int hours;
	int minutes;
	int seconds;
} Time;
typedef enum {
	HOURS,
	MINUTES,
	SECONDS
} TimeMetric;

char* int_to_string(int val) {
	if (val == 0) {
		return "0";
	}

	int num_of_digits = (int) log10((double) val) + 1;
	char* output = (char*) malloc((num_of_digits + 1) * sizeof(char));
	int size_filled_output = num_of_digits;

	output[size_filled_output--] = '\0';

	while (val > 0) {
		char curr_digit = (val % 10) + '0';
		val /= 10;
		output[size_filled_output--] = curr_digit;
	}

	return output;
}
void make_time_string_double_digits(char** time) {
	if ((*time)[1] == '\0') {
		// dynamic memory allocation is required so that the memory exists even out of scope of this function.
		char* new_time = (char*) malloc(3 * sizeof(char));

		strcpy(new_time, "0");
		strcat(new_time, *time);
		free(*time);

		*time = new_time;
	}
}
// the time we are getting here must be correctly formatted.
char* time_to_string(const Time* time) {
	char* hours = int_to_string(time -> hours);
	char* minutes = int_to_string(time -> minutes);
	char* seconds = int_to_string(time -> seconds);

	make_time_string_double_digits(&hours);
	make_time_string_double_digits(&minutes);
	make_time_string_double_digits(&seconds);

	// we will at least print two digits for every time metric and there will be 2 ':' and there will be one last space for null terminator '\0'.
	size_t total_length = strlen(hours) + strlen(minutes) + strlen(seconds) + 3;
	char* output = (char *) malloc((total_length + 1) * sizeof(char));

	snprintf(output, total_length, "%s:%s:%s", hours, minutes, seconds);

	free(hours);
	free(minutes);
	free(seconds);

	return output;
}
void add_time(Time* curr, const Time* other) {
	curr -> seconds += other -> seconds;
	curr -> minutes += other -> minutes;
	curr -> hours += other -> hours;

	curr -> minutes += curr -> seconds / 60;
	curr -> seconds = curr -> seconds % 60;

	curr -> hours += curr -> minutes / 60;
	curr -> minutes = curr -> minutes % 60;
}
void push_back(TimeMetric* time_metric) {
	if (*time_metric == HOURS) {
		*time_metric = MINUTES;
	} else if (*time_metric == MINUTES) {
		*time_metric = SECONDS;
	}
}

int main() {
	Time time = {0, 0, 0};
	char input[100];

	printf("Enter time values (format - hh:mm::ss, if any value is 0 you may skip it like 00:27:00 can be :27:, you still have to put the colons) with spaces between them (enter 'break' if you want to stop): ");

	do {
		TimeMetric curr_time_metric = HOURS;
		Time other_time = {0, 0, 0};
		int input_iterator = 0;
		int curr_val = 0;

		scanf("%99s", input);

		if (input[0] == '-') {
			printf("Invalid time format provided %s\n", input);
			break;
		}

		while (input[input_iterator] != '\0') {
			if (input[input_iterator] == ':') {
				if (curr_time_metric == HOURS) {
					other_time.hours += curr_val;
				} else if (curr_time_metric == MINUTES) {
					other_time.minutes += curr_val;
				} else {
					other_time.seconds += curr_val;
				}

				curr_val = 0;
				push_back(&curr_time_metric);
			} else if (input[input_iterator] >= '0' && input[input_iterator] <= '9') {
				curr_val = (curr_val * 10) + ((int) input[input_iterator] - '0');
			}

			++input_iterator;
		}

		other_time.seconds += curr_val;

		add_time(&time, &other_time);
	} while (strcmp(input, "break") != 0);

	char* time_string = time_to_string(&time);
	printf("%s\n", time_string);
	free(time_string);

	return 0;
}