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

void driver_monitor::DriverStatus_Drowsy(unsigned int threshold, std::vector<int> &instance_count, QString &main_string)
{
    DriverStatus_string.clear();
    if(instance_count.size() >= threshold)
    {
        DriverStatus_string = "Drowsy";
        instance_count.clear();
    }
    if(!DriverStatus_string.isEmpty())
    {
        main_string += "[";
        main_string += time_string;
        main_string += "] - ";
        main_string += DriverStatus_string;
        main_string += "\n";
    }
}

void driver_monitor::DriverStatus_Distracted(int threshold, QTime &instance_timer, int instance_count, QString &main_string)
{
    DriverStatus_string.clear();
    static int HeadTurn_PrevCount = -1;
    if(instance_timer.elapsed() > threshold && HeadTurn_PrevCount != instance_count && status == 1)
    {
        DriverStatus_string = "Distracted";
        HeadTurn_PrevCount = instance_count;
    }
    if(!DriverStatus_string.isEmpty())
    {
        main_string += "[";
        main_string += time_string;
        main_string += "] - ";
        main_string += DriverStatus_string;
        main_string += "\n";
    }
}

void driver_monitor::DriverStatus_Asleep(int threshold, QTime &instance_timer, int instance_count, QString &main_string)
{
    DriverStatus_string.clear();
    static int Blink_PrevCount = -1;
    if(instance_timer.elapsed() > threshold && Blink_PrevCount != instance_count && status == 1)
    {
        DriverStatus_string = "Asleep";
        Blink_PrevCount = instance_count;
    }
    if(!DriverStatus_string.isEmpty())
    {
        main_string += "[";
        main_string += time_string;
        main_string += "] - ";
        main_string += DriverStatus_string;
        main_string += "\n";
    }
}
