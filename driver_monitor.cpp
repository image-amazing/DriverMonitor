#include "driver_monitor.h"

driver_monitor::driver_monitor(const dlib::full_object_detection &face_landmark)
    :shape(face_landmark)
{
}

void driver_monitor::measure(int main_pt1, int main_pt2, char main_axis, int scale_pt1, int scale_pt2, char scale_axis, int scale_factor)
{
    if (main_axis == 'x' && scale_axis == 'x')
        facial_feature = ((shape.part(main_pt1).x() - shape.part(main_pt2).x()) * scale_factor) / (shape.part(scale_pt1).x() - shape.part(scale_pt2).x());
    else if (main_axis == 'x' && scale_axis == 'y')
        facial_feature = ((shape.part(main_pt1).x() - shape.part(main_pt2).x()) * scale_factor) / (shape.part(scale_pt1).y() - shape.part(scale_pt2).y());
    else if (main_axis == 'y' && scale_axis == 'x')
        facial_feature = ((shape.part(main_pt1).y() - shape.part(main_pt2).y()) * scale_factor) / (shape.part(scale_pt1).x() - shape.part(scale_pt2).x());
    else if (main_axis == 'y' && scale_axis == 'y')
        facial_feature = ((shape.part(main_pt1).y() - shape.part(main_pt2).y()) * scale_factor) / (shape.part(scale_pt1).y() - shape.part(scale_pt2).y());
    else
        cout << "measurement axis error" << endl;
}

void driver_monitor::eye_parameters(int &max, int &min)
{
    if (max > 50)
        max = 0;
    if (max < facial_feature)
        max = facial_feature;
    if (min > facial_feature)
        min = facial_feature;
    percent = ((facial_feature - 10) * 100) / (max - 10);

}

void driver_monitor::head_parameters(int main_side, int minor_side)
{
    int max;
    max = main_side + minor_side;
    percent = (main_side * 100) / max;
}

void driver_monitor::instance(char inst, bool &instance_trigger, QTime &instance_timer, int parameter1, int threshold1, int parameter2, int threshold2, int parameter3, int threshold3)
{
    switch(inst)
    {
    case 'b':
        if(parameter1 <= threshold1 || parameter2 <= threshold2)
        {
            if(instance_trigger == false)
                instance_timer.start();
            status = 1;
            instance_trigger = true;
        }
        else
        {
            status = 0;
        }
        break;
    case 'y':
        if(parameter1 > threshold1 && (parameter2 <= threshold2 || parameter3 <= threshold3))
        {
            if(instance_trigger == false)
                instance_timer.start();
            status = 1;
            instance_trigger = true;
        }
        else
        {
            status = 0;
        }
        break;
    case 'h':
        if(parameter1 > threshold1)
        {
            if(instance_trigger == false)
                instance_timer.start();
            status = 1;
            instance_trigger = true;
        }
        else
        {
            status = 0;
        }
        break;
    }
}

void driver_monitor::shift_register(std::vector<int> &sample_vector, unsigned int size, bool print)
{
    sample_vector.insert(sample_vector.begin(), status);
    if(sample_vector.size() > size)
        sample_vector.pop_back();
    count = 0;
    for(unsigned int i = 0; i < sample_vector.size(); i++)
    {
        if(sample_vector.at(i) == 1)
            count++;
        if(print == true)
            cout << sample_vector.at(i);
    }
    if(print == true)
        cout << endl;
}

void driver_monitor::classify(std::vector<int> &sample_vector, char inst, bool &instance_trigger)
{
    if(count != 0 && instance_trigger == true)
    {
        if(sample_vector[0] == 0 && sample_vector[1] == 0 && sample_vector[2] == 0)
        {
            switch(inst)
            {
            case 'b':
                string_status = "Blink";
                break;
            case 'y':
                string_status = "Yawn";
                break;
            case 'l':
                string_status = "Head turn left";
                break;
            case 'r':
                string_status = "Head turn right";
                break;
            }
            instance_trigger = false;
        }
    }
}

void driver_monitor::guiDisplay_text(QString &instance_string, QTime instance_timer, QString &separate, int &total_count)
{
    if(!string_status.empty())
    {        
        total_count++;
        instance_time = instance_timer.elapsed();

        instance_string.append("[");
        instance_string.append(time_string);
        instance_string.append("] - ");
        instance_string.append(QString::fromStdString(string_status));
        instance_string.append(" ");
        instance_string.append(QString::number(instance_time));
        instance_string.append(" ms\n");

        separate.append(QString::number(total_count));
        separate.append(". [");
        separate.append(time_string);
        separate.append("] - ");
        separate.append(QString::fromStdString(string_status));
        separate.append(" ");
        separate.append(QString::number(instance_time));
        separate.append(" ms\n");
    }
}

void driver_monitor::instance_rate(std::vector<int> &instance_count, int time_rate, int threshold_time)
{
    current_time_seconds = (time.hour() * 60 * 60) + (time.minute() * 60) + time.second();
    if(!string_status.empty())
    {
        if(instance_time > threshold_time)
            instance_count.push_back(current_time_seconds);
    }

    for(unsigned int i = 0; i < instance_count.size(); i++)
    {
        if((current_time_seconds - instance_count.front()) > time_rate)
            instance_count.erase(instance_count.begin());
    }
}
