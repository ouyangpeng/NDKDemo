package com.oyp.ndkdemo

import android.graphics.PointF

class FaceFeatureBean(
    var faceId: Int,
    var boundingBox: Array<PointF>,
    var landmarks: Array<PointF>,
    var visibilities: List<Float>,
    var yaw: Float,
    var pitch: Float,
    var roll: Float
)