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

void driver_monitor::classify(char inst, bool &instance_trigger, QTime instance_timer, int threshold_time, bool &display_trigger)
{
    if(instance_trigger == true && status == 0)
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
        if(instance_timer.elapsed() > threshold_time)
        {
            display_trigger = true;
            instance_time = instance_timer.elapsed();
        }
    }

}

void driver_monitor::instance_rate(std::vector<int> &instance_count, int time_span, int threshold_time, bool &display_trigger)
{
    current_time_seconds = (time.hour() * 60 * 60) + (time.minute() * 60) + time.second();
    if(display_trigger == true && instance_time > threshold_time)
    {
        instance_count.push_back(current_time_seconds);
    }

    for(unsigned int i = 0; i < instance_count.size(); i++)
    {
        if((current_time_seconds - instance_count.front()) > time_span)
            instance_count.erase(instance_count.begin());
    }
}

void driver_monitor::guiDisplay_text(QString &main_string,  QString &instance_string, QTime instance_timer, int &count, bool &display_trigger)
{
    if(display_trigger == true)
    {        
        count++;
        //instance_time = instance_timer.elapsed();

        //Time stamp
        main_string += "[";
        main_string += time_string;
        main_string += "] - ";
        main_string += QString::fromStdString(string_status);
        main_string += " ";
        main_string += QString::number(instance_time);
        main_string += " ms\n";

        instance_string += QString::number(count);
        instance_string += ". [";
        instance_string += time_string;
        instance_string += "] - ";
        instance_string += QString::fromStdString(string_status);
        instance_string += " ";
        instance_string += QString::number(instance_time);
        instance_string += " ms\n";

        display_trigger = false;
    }
}

void driver_monitor::driver_status(std::vector<int> &instance_count, unsigned int threshold, char type)
{
    if(instance_count.size() >= threshold)
    {
        string status;
        switch(type)
        {
        case 'a':
            status = "drowsy";
            break;
        case 'b':
            status = "distracted";
            break;
        }
        cout << status << endl;
        instance_count.clear();
    }
}
